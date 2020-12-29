#include<util.h>
#include<stdbool.h>
#include<stdio.h>
#include<testUtil.h>
#include<errCodes.h>
#include<dbQuery.h>

bool testNoOfChars(){

	char wordlist[4][10] ={"donald","Christmas","999",""};
	int  expectedOutcome[] = {6,9,3,0};

	int index = 0;

	while (index < 4){
		
		if( noOfChars(wordlist[index]) == expectedOutcome[index++] ){
			continue;
		}
		else{
			printf("expeted %d recieved %d", expectedOutcome[index],noOfChars(wordlist[index]));
			return false;
		}
		index++;
	}
	return true;

}

void testDbGetResponseUrlForUser(void){

	char userList[4][10] = {"jacob","srini","jyoti","prabjot"};
	int index = 0;
	char *url;
	while(index < 4){

		dbGetResponseUrlForUser( userList[index], url);
		index++;
	}

}
