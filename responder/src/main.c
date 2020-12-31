//================================================
// 
// Title : A fastCgi Response Handling application
//      Optimising throuput by caching
// Author : Vineet Uprari
// Date : 23/12/2020
//
//================================================



#include<stdio.h>
#include<util.h>
#include<db.h>
#include<dbQuery.h>
#include<testUtil.h>
#include<errCodes.h>
#include<cache.h>
#include<cacheQuery.h>

#define SUCCESS 0
#define FAILURE 1
#define MAX_RESPONSE_SIZE 100
int main()
{

    tErrCode err;

    printf("Bootcamp 1.0\n");
    int firstNumber, secondNumber, result;

    printf("please enter two numberi\n");
    scanf("%d %d", &firstNumber, &secondNumber);

    printf("The values recieved are %d , %d\n", firstNumber, secondNumber);
    result = utilAddTwoNumbers(firstNumber, secondNumber);
    printf("Sum of the two number's is %d\n", result);
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
	char *cacheResponseUrl = NULL;
    char *queryUserName = "raju";
    dbGetResponseUrlForUser(queryUserName, responseUrl);
	cacheSetValue( queryUserName, responseUrl);

	cacheGetValue(queryUserName, &cacheResponseUrl);
	if(cacheResponseUrl != NULL ){
		printf("cache: value recieved: %s for key %s\n",cacheResponseUrl,queryUserName);
		free(cacheResponseUrl);
	}
    	
    testDbGetResponseUrlForUser();
    testNoOfChars();
    //TODO :-
    //     CLEAN DB AND CLOSE IT
    return SUCCESS;
}
