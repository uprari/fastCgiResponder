#include<util.h>
#include<stdbool.h>
#include<stdio.h>
#include<testUtil.h>
#include<errCodes.h>
#include<db.h>
#include<dbQuery.h>
#include<cache.h>
#include<cacheQuery.h>
#include<main.h>

bool testNoOfChars()
{

    char wordlist[4][10] = { "donald", "Christmas", "999", "" };
    int expectedOutcome[] = { 6, 9, 3, 0 };

    int index = 0;

    while (index < 4) {

	if (utilNoOfChars(wordlist[index]) == expectedOutcome[index++]) {
	    continue;
	} else {
	    printf("expeted %d recieved %d", expectedOutcome[index],
		   utilNoOfChars(wordlist[index]));
	    return false;
	}
	index++;
    }
    return true;

}

void testDbGetResponseUrlForUser(void)
{

    char userList[4][10] = { "jacob", "srini", "jyoti", "prabjot" };
    int index = 0;
    char url[100];
    while (index < 4) {

	dbGetResponseUrlForUser(userList[index], url);
	index++;
    }

}


int testDbCacheQueries()
{


    tErrCode err;
    printf("Bootcamp 1.0\n");
    int firstNumber, secondNumber, result;

    printf("please enter two numberi\n");
    scanf("%d %d", &firstNumber, &secondNumber);

    printf("The values recieved are %d , %d\n", firstNumber, secondNumber);
    result = utilAddTwoNumbers(firstNumber, secondNumber);
    printf("Sum of the two number's is %d\n", result);
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
    char responseUrl[MAX_RESPONSE_SIZE];
    char *cacheResponseUrl = NULL;
    char *queryUserName = "raju";
    dbGetResponseUrlForUser(queryUserName, responseUrl);
    cacheSetValue(queryUserName, responseUrl);

    cacheGetValue(queryUserName, &cacheResponseUrl);
    if (cacheResponseUrl != NULL) {
	printf("cache: value recieved: %s for key %s\n", cacheResponseUrl,
	       queryUserName);
	free(cacheResponseUrl);
    }

    testDbGetResponseUrlForUser();
    testNoOfChars();
    //TODO :-
    //     CLEAN DB AND CLOSE IT

}

void testGetQueryForUser()
{

    char testQuery[4][50] =
	{ "user=3abc", "abc=123&user=abc", "xys=123&abc=444&user=hello",
	"user=abhishek&abc=213"
    };
    char *user;
    int indx = 0;

    while (*testQuery[indx] != '\0') {
	GET_USER_FROM_QUERY(testQuery[indx], user);
	indx++;
	//getUserFromQuery(testQuery[index++], &user);
	printf("the user name is %s\n", user);
    }
}
