#ifndef ERR_CODES
#define ERR_CODES
#include<stdio.h>
#define MAX_DESCRIPTION_LENGTH 100

enum responderErrCodes {

    NO_ERROR = 0,
    INVALID_CONFIG_SERVER_ABSENT = 1,
    INVALID_CONFIG_DB_ABSENT,
    INVALID_CONFIG_USER_ABSENT,
    DB_SERVER_CONNECTION_FAILURE,
    REDIRECTION_URL_FETCH_FAILURE,
    REDIRECTION_URL_FETCH_RESULT_STORE_FAILURE,
    QUERY_FORMATION_ERROR,
    INVALID_QUERY_USER_NOT_FOUND,
    DB_RECORD_ABSENT,
    MAX_VALUE
};

typedef enum responderErrCodes tErrCode;


char *errDescription[MAX_VALUE];

void utilInit(void);

//LOGGING STRUCTURE
//FILE *logFileDesc;
//#define LogFilePath "../log/log_responder.txt"
#define RESPONDER_LOG(format, ...)
// fprintf( stderr , format , ##__VA_ARGS__)


#endif
