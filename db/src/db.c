#include<stdio.h>
#include<mysql.h>
#include<db.h>

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
        
	// reading config file
	FILE * filePtr = NULL;
	char buffer[1000];
	
	if((filePtr = fopen("../../config/config.txt", "r")) == NULL ) {
		printf("ERROR! Opening file\n");
		printf("Configuration reading failed\n");
                return;
	}

	return;
}
