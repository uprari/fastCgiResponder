//================================================
// 
// Title : A fastCgi Response Handling application
//	Optimising throuput by caching
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

#define SUCCESS_ 0
#define FAILURE_ 1

int main(){

	tErrCode err;

	printf("Bootcamp 1.0\n");
	int firstNumber,secondNumber, result;
	
	printf("please enter two numberi\n");
	scanf("%d %d",&firstNumber,&secondNumber);

	printf("The values recieved are %d , %d\n", firstNumber, secondNumber);
	result = utilAddTwoNumbers(firstNumber,secondNumber);
	printf("Sum of the two number's is %d\n",result);
	initErrorCodes();
        if( ( err = dbInitialise() ) != SUCCESS ){
		printf("database initialisation failed due to err : %s\n", errDescription[err]);
		return FAILURE_;
	}

	char *responseUrl = NULL;
	char *queryUserName = "raju";
	dbGetResponseUrlForUser( queryUserName, responseUrl);
	
	testDbGetResponseUrlForUser();	
	testNoOfChars();
	return SUCCESS_;
}
