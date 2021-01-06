#include <stdio.h>
#include <mysql.h>
#include <errCodes.h>
#include <dbQuery.h>
#include <db.h>
#include <string.h>

tErrCode dbGetResponseUrlForUser(char *username, char *url)
{

	int len = 0;
	len = sprintf(glbQueryString, REDIRECTION_URL_QUERY_FORMAT, username);

	if (len == 0) {
		return QUERY_FORMATION_ERROR;
	}
	if (mysql_query(dbInfo.conn, glbQueryString)) {

		printf("Failed to query from database %s Error: %s\n",
				dbConfig.database, mysql_error(dbInfo.conn));
		return REDIRECTION_URL_FETCH_FAILURE;

	}

	MYSQL_RES *result = mysql_store_result(dbInfo.conn);
	if (result == NULL) {

		printf("Failed to store result Error: %s\n",
				mysql_error(dbInfo.conn));
		return REDIRECTION_URL_FETCH_RESULT_STORE_FAILURE;

	}
	if(result->row_count == 1){
		MYSQL_ROW row;
		row = mysql_fetch_row(result);
		strcpy(url,row[0]);
		printf("url retrieved %s for user : %s\n", url, username);
		mysql_free_result(result);
		return NO_ERROR;
	} else {
		printf("user not found: %s \n", username);
	}
	mysql_free_result(result);
	return DB_RECORD_ABSENT;
}
