#ifndef MAIN_H
#define MAIN_H

#include<errCodes.h>
#include <libmemcached/memcached.h>
#include <mysql.h>

#define SUCCESS 0
#define FAILURE 1
#define MAX_RESPONSE_SIZE 100

typedef struct threadCtx{
	int threadID;
	memcached_st *memc;
	MYSQL *conn;
}threadCTX;

int mainInit(void);
tErrCode readConfig(void);
void *handlerFunction(void*);
void mainAddConfigValue(char *buffer, int key, int val);


#define  GET_USER_FROM_QUERY(queryString, user) ({ \
    char *key; \
	char *searchKey = "user";\
	int len = 4;\
	int index = 0;\
	int sIndex = 0;\
	bool found = false;\
	int valueLen = 0;\
    bool skipKey = false;\
\
	key = queryString;\
	while(key[index] != '\0'){\
\
		sIndex = 0;	\
		skipKey = false;\
		while(key[index] != '\0' && sIndex < len){ \
\
			if(key[index++] != searchKey[sIndex++]){\
				skipKey = true ;\
				break;\
			}\
		}\
		if (skipKey){\
			while(key[index] != '\0' && key[index] !='&'){\
				index++;\
			}\
			if(key[index] == '&'){\
				index++;\
			}\
			continue;\
		}\
		if(sIndex == len && key[index++] == '=' ){\
           \
			key = key + index;\
			index = 0;\
			while(key[index]!= '\0' && key[index]!='&'){\
				index++;\
			}\
			if(index < MAX_RESPONSE_SIZE){\
				user = key;\
			}\
			if ( key[index] != '\0' ){\
				key[index]='\0';\
			}\
			break;\
		}\
	}\
})
#endif
