#include<errCodes.h>

#define SUCCESS 0
#define FAILURE 1
#define MAX_RESPONSE_SIZE 100

tErrCode getUserFromQuery(char *queryString, char *user);
int mainInit();
