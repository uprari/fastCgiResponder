#include<stdio.h>
#include<mysql.h>
#include<db.h>
#include<stdbool.h>
#include<util.h>
#include<errno.h>
#include<string.h>

#define dbConfigfile "../config/dbConfig.txt"

//global data structures
globalDbInfo dbInfo;
globalDbConfig dbConfig;

void dbInitialise(){

	dbReadConfig();

}

void dbReadConfig(){

	//initialising global Data Structures;
	dbInfo.conn = NULL;
	dbInfo.res  = NULL;
	dbInfo.row  = NULL;

	dbConfig.server = NULL;
	dbConfig.user   = NULL;
	dbConfig.password = NULL;
	dbConfig.database = NULL;
        
	captureConfig( dbConfigfile, addConfigValue);	
	printGlobalConfig();
	return;
}


void addConfigValue(char *buffer, int key, int val){
	const char* bufval = buffer + val ;
	const char* bufkey = buffer + key;
	if(strcmp(bufkey,"user") == 0){
           dbConfig.user = (char *)malloc(noOfChars(bufval));
	   strcpy(dbConfig.user,bufval);          
	}
	else if(strcmp(bufkey,"server") == 0 ){
           dbConfig.server = (char *)malloc(noOfChars(bufval));
	   strcpy(dbConfig.server,bufval);          
	}
	else if(strcmp(bufkey,"password") == 0){
           dbConfig.password = (char *)malloc(noOfChars(bufval));
	   strcpy(dbConfig.password,bufval);          
	}
	else if(strcmp(bufkey,"database") == 0){
           dbConfig.database = (char *)malloc(noOfChars(bufval));
	   strcpy(dbConfig.database,bufval);          

	}
}

void printGlobalConfig(){

	if(dbConfig.user != NULL){
	 	printf("user: %s\n",dbConfig.user);
	}
	if(dbConfig.server != NULL){
	 	printf("server: %s\n",dbConfig.server);
	}
	if(dbConfig.password != NULL){
	 	printf("password: %s\n",dbConfig.password);
	}
	if(dbConfig.database != NULL){
	 	printf("database: %s\n",dbConfig.database);
	}
}
