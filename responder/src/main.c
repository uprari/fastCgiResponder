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
#include<util.h>
#include<db.h>
#include<dbQuery.h>
#include<testUtil.h>
#include<errCodes.h>
#include<cache.h>
#include<cacheQuery.h>
#include<fcgi_stdio.h>

#define SUCCESS 0
#define FAILURE 1
#define MAX_RESPONSE_SIZE 100
int main()
{
	char *queryString;
	char user[MAX_RESPONSE_SIZE];
	char responseUrl[MAX_RESPONSE_SIZE] ;
	char *cacheResponseUrl = NULL;
	tErrCode err;
	if ( mainInit() == FAILURE )
		return FAILURE;
	/*	int result = FAILURE;
		if ( (result = testDbCacheQueries()) == FAILURE ){
		printf("Test DB CACHE QUERIES failed\n");
		}*/
	//FCGI INTEGRATION
	testGetQueryForUser();

	while(FCGI_Accept() >= 0 ){

		printf("Content-type: text/html\r\n\r\n");

		printf("<title>Prime FastCgi</title>\n<h1> Vineet Uprari FastCgi assignment :)</h1>\n");
		queryString = getenv("QUERY_STRING");
		if(queryString == NULL ){

			printf("<h1>please giver user=\"username\" as query string</h1>\n");
			return SUCCESS;

		}
		if( (err = getUserFromQuery(queryString,user)) == INVALID_QUERY_USER_NOT_FOUND ){

			printf("<h1>Get Query Error %s </h1>", errDescription[err]);
			return FAILURE;
		}
		printf("<h1>The user identified from query is %s </h1>",user);

		cacheGetValue(user, &cacheResponseUrl);
		if(cacheResponseUrl != NULL ){
			printf("<h1>[cache HIT]cache: value recieved: %s for key %s</h1>\n",cacheResponseUrl,user);
			free(cacheResponseUrl);
			continue;
		}

		//put macro comparissons on the left hand side	
		if( (err = dbGetResponseUrlForUser(user, responseUrl) ) == NO_ERROR){
			printf("<h1>[DB HIT]Database: value recieved: %s for key %s</h1>\n", responseUrl, user);
		}else{
			printf("<h1>[DB failure]Database Error:%s , User Queried :: %s</h1>\n",errDescription[err], user);
			continue;
		}

		cacheSetValue( user, responseUrl);

		printf("<h1>The query string recieved is %s </h1>\n",queryString);	
	}

	return SUCCESS;
}


tErrCode getUserFromQuery(char *queryString, char *user){

	char *key;
	char *searchKey = "user";
	int len = 4;
	int index = 0;
	bool found = false;
	int valueLen = 0;

	key = strtok(queryString,"&");

	while(key != NULL){

		index = 0;	
		while(key[index] != '\0' && index < len){

			if(key[index] != searchKey[index++])
				break;

		}
		if(index == len && key[index++] == '=' ){

			valueLen = sizeof(key+index);
			if(valueLen < MAX_RESPONSE_SIZE){
				strcpy(user,key+index);
				return NO_ERROR;
			}

		}
		key = strtok(NULL, "&");
	}

	return 	INVALID_QUERY_USER_NOT_FOUND;	
}

int mainInit(){

	tErrCode err;
	utilInitErrorCodes();

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
	char responseUrl[MAX_RESPONSE_SIZE] ;

	return SUCCESS;
}
