#define MAX_QUERY_SIZE 100
#define REDIRECTION_URL_QUERY_FORMAT "select redirection_url from redirect_tbl where user_name = '%s' "

char glbQueryString[MAX_QUERY_SIZE];

tErrCode dbGetResponseUrlForUser(char *username, char* url);
tErrCode thDbGetResponseUrlForUser(MYSQL *conn,char *username, char *url);
