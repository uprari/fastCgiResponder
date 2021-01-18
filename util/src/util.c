#include<stdio.h>
#include<util.h>
#include<stdbool.h>
#include<errno.h>
#include<string.h>
#include<errCodes.h>

int utilAddTwoNumbers(int a, int b)
{

	return a + b;

}

int utilNoOfChars(char *word)
{

	if (word == NULL) {

		return 0;
	}

	int count = 0;
	while (*word != '\0' && *word != ' ' && *word != '\n' && *word != '\t') {
		count++;
		word++;
	}
	*word = '\0';

	return count;
}

int utilEscapeSpace(char *buffer, int index)
{

	while (buffer[index] == ' ' || buffer[index] == '\t')
		index++;
	return index;

}

bool utilExtractKeyVal(char *buf, int *k, int *v, int index)
{

	if (buf[index] == '\0')
		return false;
	*k = index;

	while (buf[index] != '\0') {
		if (buf[index++] != ' ')
			continue;
		buf[index - 1] = '\0';
		break;
	}
	utilEscapeSpace(buf, index);
	if (buf[index] != '\0')
		*v = index;

	if (*v != 0) {
		return true;
	}
	return false;
}

	void
utilCaptureConfig(char *filePath, void (*configAdd) (char *, int, int))
{

	if (filePath == NULL || configAdd == NULL) {
		printf("NUll Exception CaptureConfig");
	}
	// reading config file
	FILE *filePtr = NULL;
	char buffer[1000];

	if ((filePtr = fopen(filePath, "r")) == NULL) {
		printf("ERROR! Opening file\n");
		printf("Configuration reading failed, with error no %d: %s \n",
				errno, strerror(errno));
		return;
	}


	int key, val, index;

	while ((fgets(buffer, MAX_LINE_SIZE, filePtr)) != NULL) {

		key = val = index = 0;
		if (buffer[index] == '#')
			continue;
		index = utilEscapeSpace(buffer, index);
		if (utilExtractKeyVal(buffer, &key, &val, index) == false)
			continue;

		configAdd(buffer, key, val);
	}
}

void utilInit(void)
{
   	//logFileDesc = fopen(LogFilePath, "a");
    //initing error description`
	errDescription[INVALID_CONFIG_SERVER_ABSENT] =
		"Server name absent from config file";
	errDescription[INVALID_CONFIG_DB_ABSENT] =
		"Db name absent from config file";
	errDescription[INVALID_CONFIG_USER_ABSENT] =
		"Db User name absent from config file";
	errDescription[DB_SERVER_CONNECTION_FAILURE] =
		"Connection to Db Failed";
	errDescription[REDIRECTION_URL_FETCH_FAILURE] =
		"Unable to fetch redirection url from database";
	errDescription[REDIRECTION_URL_FETCH_RESULT_STORE_FAILURE] =
		"Unable to store result from db";
	errDescription[QUERY_FORMATION_ERROR] =
		"Error while forming query for database";
	errDescription[DB_SERVER_CONNECTION_FAILURE] =
		"Connection to Db Failed";
	errDescription[INVALID_QUERY_USER_NOT_FOUND] =
		"user not found in HTTP GET query request";
	errDescription[DB_RECORD_ABSENT] = "user not found in Database";
	errDescription[MEMCACHE_CONNECTION_ERROR] = "Cache failed to connect";

}

tErrCode getUserFromQuery(char *queryString, char **user){

	char *key;
	char *searchKey = "user";
	int len = 4;
	int index = 0;
	int sIndex = 0;
	bool found = false;
	int valueLen = 0;
    bool skipKey = false;

	key = queryString;
	while(key[index] != '\0'){

		sIndex = 0;	
		skipKey = false;
		while(key[index] != '\0' && sIndex < len){

			if(key[index++] != searchKey[sIndex++]){
				skipKey = true ;
				break;
			}
		}
		if (skipKey){
			while(key[index] != '\0' && key[index] !='&'){
				index++;
			}
			if(key[index] == '&'){
				index++;
			}
			continue;
		}
		if(sIndex == len && key[index++] == '=' ){
           
			key = key + index; //update key position to value
			index = 0;
			while(key[index]!= '\0' && key[index]!='&'){
				index++;
			}
			if(index < MAX_RESPONSE_SIZE){
				*user = key;
			}
			if ( key[index] != '\0' ){
				key[index]='\0';
			}
			return NO_ERROR;
		}
		
	}

	return 	INVALID_QUERY_USER_NOT_FOUND;	
}
