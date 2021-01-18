//================================================
// 
// Title : A fastCgi Response Handling application
//      Optimising throuput by caching
// Author : Vineet Uprari
// Date : 23/12/2020
//
//================================================


#include<main.h>
#include<stdio.h>
#include<fcgi_stdio.h>
#include<util.h>
#include<db.h>
#include<dbQuery.h>
#include<testUtil.h>
#include<errCodes.h>
#include<cache.h>
#include<cacheQuery.h>
#include<pthread.h>

#define SUCCESS 0
#define FAILURE 1
#define MAX_RESPONSE_SIZE 100
#define mainConfigFile "../config/mainConfig.txt"
#define MAX_THREAD_COUNT 100

int gThreadCount;

int main()
{
	pthread_t *thread = NULL;
	threadCTX *thCtx = NULL;
	gThreadCount = 0;
	tErrCode err;

	//Reading main config file for getting
	//number of threads to spawn
	readConfig();
	int er;
	er = FCGX_Init();
	if (er != 0) {
		return 0;
	}

	if (FAILURE == mainInit())
		return FAILURE;

	// memory allocation for threads
	thread = (pthread_t *) malloc(sizeof(pthread_t) * gThreadCount);
	thCtx = (threadCTX *) malloc(sizeof(threadCTX) * gThreadCount);

	RESPONDER_LOG("\n\n RESPONDER UP AND RUNNING\n\n");
	for (int i = 0; i < gThreadCount; i++) {
		thCtx[i].threadID = i;
		pthread_create(&thread[i], NULL, handlerFunction, thCtx + i);
	}

	for (int i = 0; i < gThreadCount; i++) {

		pthread_join(thread[i], NULL);
	}

	//closing connections and cleaning memory;
	cleanUp(thread, thCtx);
	return SUCCESS;
}

int mainInit()
{

	tErrCode err = NO_ERROR;
	utilInit();
	err = dbInitialise();
	if (NO_ERROR != err ) {
		RESPONDER_LOG("database initialisation failed due to err : %s\n",
				errDescription[err]);
		return FAILURE;
	}
	err = cacheInitialise();
	if ( NO_ERROR != err) {

		RESPONDER_LOG("cache initialisation failed due to err :%s\n",
				errDescription[err]);
		return FAILURE;
	}

	return SUCCESS;

}

void *handlerFunction(void *thContex)
{

	char *queryString;
	char *user;
	char responseUrl[MAX_RESPONSE_SIZE];
	char *cacheResponseUrl = NULL;
	FCGX_Request request;
	threadCTX *threadCtx = (threadCTX *) thContex;
	int rc = 0;
	tErrCode err = NO_ERROR;
	// FCGI initialize request
	FCGX_InitRequest(&request, 0, 0);
	thCacheConnect(&(threadCtx->memc));
	thDbConnect(&(threadCtx->conn));
	RESPONDER_LOG
		("Thread %d sitting in infinite loop\n", threadCtx->threadID);
	while (1) {
		rc = FCGX_Accept_r(&request);
		if (rc < 0) {
			RESPONDER_LOG
				(" FCGX_Accept_r() failed with return value = %d,"
				 "Thread Ending infinite loop %s:%d\n",
				 rc, __FILE__, __LINE__);
			break;
		}
		queryString = FCGX_GetParam("QUERY_STRING", request.envp);
		if ( NULL == queryString) {
			printf("invalid query string\n");
			goto failResponse;
		}
#if 0
		GET_USER_FROM_QUERY(queryString, user);
#endif
#if 1
		err = getUserFromQuery(queryString, &user);
		if ( INVALID_QUERY_USER_NOT_FOUND == err ) {
			printf("userNotfound in the string\n");
			RESPONDER_LOG
				("Thread %d: user key not found in query\n",
				 threadCtx->threadID);
			goto failResponse;
		}
#endif
		thCacheGetValue(threadCtx->memc, user, &cacheResponseUrl);
		if ( NULL != cacheResponseUrl ) {
			//FCGX_FPrintF(request.out, "Location: %s\r\n\r\n", cacheResponseUrl);
			RESPONDER_LOG
				("Thread %d: cache HIT : Found %s\n",
				 threadCtx->threadID, cacheResponseUrl);
			//FCGX_FPrintF(request.out,"<br>Redirecting to %s</br>\n",cacheResponseUrl);
			//FCGX_FPrintF(request.out, "Refresh: 0;URL=http://%s\r\n\r\n",cacheResponseUrl);
			FCGX_FPrintF(request.out,
					"Content-type: text/html; charset=utf-8\r\n\r\n");
			FCGX_FPrintF(request.out,
					"<head>\n<meta http-equiv=\"refresh\" content=\"1;url=http://%s\" />\n</head>",
					cacheResponseUrl);
			//FCGX_FPrintF(request.out, "\nStatus: %d %s\r\n\r\n", 200, "ok");
			goto sendResponse;
		}
		err =
			thDbGetResponseUrlForUser(threadCtx->conn, user, responseUrl);
		if ( NO_ERROR == err ) {
			//FCGX_FPrintF(request.out, "Location: %s\r\n\r\n", responseUrl);
			RESPONDER_LOG
				("Thread %d: db HIT : Found %s\n",
				 threadCtx->threadID, responseUrl);
			//FCGX_FPrintF(request.out,"<br>Redirecting to %s</br>\n",responseUrl);
			FCGX_FPrintF(request.out,
					"Refresh: 0;URL=http://%s\r\n\r\n", responseUrl);
			//FCGX_FPrintF(request.out, "Content-type: text/html; charset=utf-8\r\n\r\n");
			//FCGX_FPrintF(request.out,"<meta http-equiv=\"refresh\" content=\"0;url=%s\" />",responseUrl);
			//FCGX_FPrintF(request.out, "Status: %d %s\r\n\r\n", 200, "ok");
			thCacheSetValue(threadCtx->memc, user, responseUrl);
			goto sendResponse;
		}
failResponse:
		FCGX_FPrintF(request.out,
				"Content-type: text/html; charset=utf-8\r\n\r\n");
		FCGX_FPrintF(request.out, "<h1>user:%s not found</h1>\r\n", user);
		FCGX_FPrintF(request.out, "Status: %d %s\r\n\r\n", 404,
				"user not found in db");
		RESPONDER_LOG("Thread %d: user %s not found in db\n",
				threadCtx->threadID, user);
sendResponse:
		FCGX_Finish_r(&request);
		free(cacheResponseUrl);
	}
}


tErrCode readConfig()
{

	utilCaptureConfig(mainConfigFile, mainAddConfigValue);
	mainConfigValidate();
	return NO_ERROR;
}

void mainAddConfigValue(char *buffer, int key, int val)
{
	char *bufval = buffer + val;
	char *bufkey = buffer + key;
	int size = utilNoOfChars(bufval);
	// define macros for string litterals thread, server...
	if ( 0 == strcmp(THREAD, bufkey)) {
		gThreadCount = atoi(bufval);
	}
	//[comment thread error handling and put default value in validation code for config read]
}

void cleanUp(pthread_t * thread, threadCTX * thCtx)
{

	RESPONDER_LOG("Cleaning ...\n");
	for (int i = 0; i < gThreadCount; i++) {
		cacheClean(thCtx[i].memc);
		dbClean(thCtx[i].conn);
	}
	free(thread);
	free(thCtx);
}

void mainConfigValidate()
{

	if ( MIN_THREAD_COUNT > gThreadCount) {

		RESPONDER_LOG
			("setting thread count to minimum thread system requirement\n thread count recieved from config:%d\n",
			 gThreadCount);
		gThreadCount = MIN_THREAD_COUNT;
	}
	if  ( MAX_THREAD_COUNT < gThreadCount ) {

		RESPONDER_LOG
			("setting thread count to maximum thread allowed\n thread count recieved from config:%d\n",
			 gThreadCount);
		gThreadCount = MAX_THREAD_COUNT;
	}

	RESPONDER_LOG("No of Threads set %d \n", gThreadCount);
}
