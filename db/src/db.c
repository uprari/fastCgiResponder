#include<stdio.h>
#include<mysql.h>
#include<db.h>
#include<stdbool.h>
#include<util.h>
#include<errno.h>
#include<string.h>
#include<errCodes.h>

#define dbConfigfile "../config/dbConfig.txt"

tErrCode dbInitialise()
{

	return dbReadConfig();

}

tErrCode dbReadConfig()
{

	tErrCode err;
	//initialising global Data Structures;
	dbInfo.conn = NULL;
	dbInfo.res = NULL;
	dbInfo.row = NULL;

	dbConfig.server = NULL;
	dbConfig.user = NULL;
	dbConfig.password = NULL;
	dbConfig.database = NULL;

	utilCaptureConfig(dbConfigfile, addConfigValue);

	printGlobalConfig();
	//validating config file
	if ((err = dbConfigValidate()) != NO_ERROR) {

		return err;
	}
	//establisting database connection
	dbInfo.conn = mysql_init(NULL);
	if ((err = dbConnect()) != NO_ERROR) {

		return err;
	}

	return NO_ERROR;
}


void addConfigValue(char *buffer, int key, int val)
{
	char *bufval = buffer + val;
	char *bufkey = buffer + key;
	int size = utilNoOfChars(bufval);
	if (strcmp(bufkey, "user") == 0) {
		dbConfig.user = (char *) malloc(size + 1);
		strncpy(dbConfig.user, bufval, size);
	} else if (strcmp(bufkey, "server") == 0) {
		dbConfig.server = (char *) malloc(size + 1);
		strncpy(dbConfig.server, bufval, size);
	} else if (strcmp(bufkey, "password") == 0) {
		dbConfig.password = (char *) malloc(size + 1);
		strncpy(dbConfig.password, bufval, size);
	} else if (strcmp(bufkey, "database") == 0) {
		dbConfig.database = (char *) malloc(size + 1);
		strncpy(dbConfig.database, bufval, size);

	}
}

void printGlobalConfig()
{

	if (dbConfig.user != NULL) {
		printf("user: %s\n", dbConfig.user);
	}
	if (dbConfig.server != NULL) {
		printf("server: %s\n", dbConfig.server);
	}
	if (dbConfig.password != NULL) {
		printf("password: %s\n", dbConfig.password);
	}
	if (dbConfig.database != NULL) {
		printf("database: %s\n", dbConfig.database);
	}
}

tErrCode dbConfigValidate()
{

	if (dbConfig.user == NULL) {
		return INVALID_CONFIG_USER_ABSENT;
	}
	if (dbConfig.server == NULL) {
		return INVALID_CONFIG_SERVER_ABSENT;
	}
	if (dbConfig.database == NULL) {
		return INVALID_CONFIG_DB_ABSENT;
	}
	if (dbConfig.password == NULL) {
		dbConfig.password = "";
	}

	return NO_ERROR;
}



tErrCode dbConnect()
{

	if (!mysql_real_connect
			(dbInfo.conn, dbConfig.server, dbConfig.user, dbConfig.password,
			 dbConfig.database, 0, NULL, 0)) {

		printf("Failed to connect MySQL Server %s. Error: %s\n",
				dbConfig.server, mysql_error(dbInfo.conn));
		return DB_SERVER_CONNECTION_FAILURE;
	}
	return NO_ERROR;
}
