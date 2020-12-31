#ifndef ERR_CODES
#define ERR_CODES

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
    MAX_VALUE
};

typedef enum responderErrCodes tErrCode;


char *errDescription[MAX_VALUE];

void utilInitErrorCodes(void);
#endif
