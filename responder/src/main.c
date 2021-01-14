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
	char *queryString;
	char user[MAX_RESPONSE_SIZE];
	char responseUrl[MAX_RESPONSE_SIZE] ;
	char *cacheResponseUrl = NULL;
	pthread_t *thread = NULL;
	threadCTX *thCtx  = NULL;
	gThreadCount = 0;
	readConfig();	
	tErrCode err;
	int er;
	er = FCGX_Init();
	if ( er != 0) {
        return 0;
    }
	if ( mainInit() == FAILURE )
		return FAILURE;
	/*	int result = FAILURE;
		if ( (result = testDbCacheQueries()) == FAILURE ){
		printf("Test DB CACHE QUERIES failed\n");
		}*/
	//FCGI INTEGRATION
	testGetQueryForUser();
	thread  = (pthread_t *)malloc(sizeof(pthread_t)*gThreadCount);
    thCtx   = (threadCTX*)malloc(sizeof(threadCTX)*gThreadCount);	
	
    RESPONDER_LOG("\n\n RESPONDER UP AND RUNNING\n\n"); 
	for(int i=0; i < gThreadCount; i++){
		thCtx[i].threadID = i;
		pthread_create(&thread[i],NULL,handlerFunction, thCtx + i);
	}

    for(int i=0; i < gThreadCount; i++){

		pthread_join(thread[i],NULL);
	}
	return SUCCESS;
}

int mainInit(){

	tErrCode err;
	utilInit();

	if ((err = dbInitialise()) != NO_ERROR) {
		printf("database initialisation failed due to err : %s\n",
				errDescription[err]);
		return FAILURE;
	}

	if ((err = cacheInitialise()) != NO_ERROR) {

		printf("cache initialisation failed due to err :%s\n",
				errDescription[err]);
		return FAILURE;
	}
    
	
	return SUCCESS;
}

void *handlerFunction(void* thContex){

	char *queryString;
	char *user;
	char responseUrl[MAX_RESPONSE_SIZE] ;
	char *cacheResponseUrl = NULL;
	FCGX_Request 		request;
    threadCTX *threadCtx = (threadCTX*)thContex;
	int rc = 0 ;
	tErrCode err = NO_ERROR;

	// FCGI initialize request
	FCGX_InitRequest(&request, 0, 0);
    thCacheConnect( &(threadCtx->memc) );
	thDbConnect( &(threadCtx->conn));	

    RESPONDER_LOG("Thread %d sitting in infinite loop\n", threadCtx->threadID); 
	while(1){
		rc = FCGX_Accept_r(&request);
		if( rc < 0 ){
			RESPONDER_LOG(" FCGX_Accept_r() failed with return value = %d,"
					"Thread Ending infinite loop %s:%d\n",rc,__FILE__,__LINE__);
			break;
		}
		queryString = FCGX_GetParam("QUERY_STRING", request.envp);
		if(queryString == NULL ){
			printf("invalid query string\n");
			goto failResponse;
		}
        //GET_USER_FROM_QUERY(queryString, user);
		if( (err = getUserFromQuery(queryString,&user)) == INVALID_QUERY_USER_NOT_FOUND ){
			printf("userNotfound in the string\n");
        	RESPONDER_LOG("Thread %d: user key not found in query\n", threadCtx->threadID); 
			goto failResponse;
		}

		thCacheGetValue( threadCtx->memc, user, &cacheResponseUrl);
		if(cacheResponseUrl != NULL ){
			//FCGX_FPrintF(request.out, "Location: %s\r\n\r\n", cacheResponseUrl);
        	RESPONDER_LOG("Thread %d: cache HIT : Found %s\n", threadCtx->threadID, cacheResponseUrl); 
            //FCGX_FPrintF(request.out,"<br>Redirecting to %s</br>\n",cacheResponseUrl);
            //FCGX_FPrintF(request.out, "Refresh: 0;URL=http://%s\r\n\r\n",cacheResponseUrl);
            FCGX_FPrintF(request.out, "Content-type: text/html; charset=utf-8\r\n\r\n");
            FCGX_FPrintF(request.out,"<head>\n<meta http-equiv=\"refresh\" content=\"1;url=http://%s\" />\n</head>",cacheResponseUrl);
			//FCGX_FPrintF(request.out, "\nStatus: %d %s\r\n\r\n", 200, "ok");
			goto sendResponse;
		}
		if( (err = thDbGetResponseUrlForUser(threadCtx->conn, user, responseUrl) ) == NO_ERROR){
			//FCGX_FPrintF(request.out, "Location: %s\r\n\r\n", responseUrl);
        	RESPONDER_LOG("Thread %d: db HIT : Found %s\n", threadCtx->threadID,responseUrl); 
            //FCGX_FPrintF(request.out,"<br>Redirecting to %s</br>\n",responseUrl);
            FCGX_FPrintF(request.out, "Refresh: 0;URL=http://%s\r\n\r\n",responseUrl);
            //FCGX_FPrintF(request.out, "Content-type: text/html; charset=utf-8\r\n\r\n");
            //FCGX_FPrintF(request.out,"<meta http-equiv=\"refresh\" content=\"0;url=%s\" />",responseUrl);
			//FCGX_FPrintF(request.out, "Status: %d %s\r\n\r\n", 200, "ok");
			thCacheSetValue(threadCtx->memc, user, responseUrl);
			goto sendResponse;
		}
	failResponse:			
		FCGX_FPrintF(request.out, "Status: %d %s\r\n\r\n", 404, "user not found in db");
        RESPONDER_LOG("Thread %d: user %s not found in db\n", threadCtx->threadID,user); 
			
	sendResponse:
		FCGX_Finish_r(&request);
	    free(cacheResponseUrl);
	}

	return SUCCESS;
}


tErrCode readConfig()
	{

	utilCaptureConfig(mainConfigFile, mainAddConfigValue);
	return NO_ERROR;

}

void mainAddConfigValue(char *buffer, int key, int val)
{
	char *bufval = buffer + val;
	char *bufkey = buffer + key;
	int size = utilNoOfChars(bufval);
	// define macros for string litterals thread, server...
	if (strcmp(bufkey, "thread") == 0) {
		gThreadCount = atoi(bufval);
	}

}

