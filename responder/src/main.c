//=======================
// 
// Title : A simple program to test makefile
// Author : Vineet Uprari
// Date : 23/12/2020
//
//======================



#include<stdio.h>
#include<util.h>
#include<db.h>
#include<testUtil.h>

int main(){

	printf("Bootcamp 1.0\n");
	int firstNumber,secondNumber, result;
	
	printf("please enter two numberi\n");
	scanf("%d %d",&firstNumber,&secondNumber);

	printf("The values recieved are %d , %d\n", firstNumber, secondNumber);
	result = utilAddTwoNumbers(firstNumber,secondNumber);
	printf("Sum of the two number's is %d\n",result);
        dbInitialise();
	testNoOfChars();
}
