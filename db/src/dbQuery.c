#include <stdio.h>
#include <mysql.h>
#include <errCodes.h>
#include <dbQuery.h>
#include <db.h>
#include <string.h>

tErrCode dbGetResponseUrlForUser(char *username, char *url)
{

	int len = ZERO;
	len = sprintf(glbQueryString, REDIRECTION_URL_QUERY_FORMAT, username);

	if (ZERO == len) {
		return QUERY_FORMATION_ERROR;
	}
	if (mysql_query(dbInfo.conn, glbQueryString)) {

		RESPONDER_LOG("Failed to query from database %s Error: %s\n",
				dbConfig.database, mysql_error(dbInfo.conn));
		return REDIRECTION_URL_FETCH_FAILURE;

	}

	MYSQL_RES *result = mysql_store_result(dbInfo.conn);
	if (NULL == result) {

		RESPONDER_LOG("Failed to store result Error: %s\n",
				mysql_error(dbInfo.conn));
		return REDIRECTION_URL_FETCH_RESULT_STORE_FAILURE;

	}
	if(ONE == result->row_count){
		MYSQL_ROW row;
		row = mysql_fetch_row(result);
		strcpy(url,row[0]);
		RESPONDER_LOG("url retrieved %s for user : %s\n", url, username);
		mysql_free_result(result);
		return NO_ERROR;
	} else {
		RESPONDER_LOG("user not found: %s \n", username);
	}
	mysql_free_result(result);
	return DB_RECORD_ABSENT;
}

tErrCode thDbGetResponseUrlForUser(MYSQL *conn,char *username, char *url)
{

	int len = ZERO;
	len = sprintf(glbQueryString, REDIRECTION_URL_QUERY_FORMAT, username);

	if (ZERO == len) {
		return QUERY_FORMATION_ERROR;
	}
	if (mysql_query(conn, glbQueryString)) {

		RESPONDER_LOG("Failed to query from database %s Error: %s\n",
				dbConfig.database, mysql_error(conn));
		return REDIRECTION_URL_FETCH_FAILURE;

	}

	MYSQL_RES *result = mysql_store_result(conn);
	if (NULL == result) {

		RESPONDER_LOG("Failed to store result Error: %s\n",
				mysql_error(conn));
		return REDIRECTION_URL_FETCH_RESULT_STORE_FAILURE;

	}
	if(ONE == result->row_count){
		MYSQL_ROW row;
		row = mysql_fetch_row(result);
		strcpy(url,row[0]);
		RESPONDER_LOG("url retrieved %s for user : %s\n", url, username);
		mysql_free_result(result);
		return NO_ERROR;
	} else {
		RESPONDER_LOG("user not found: %s \n", username);
	}
	mysql_free_result(result);
	return DB_RECORD_ABSENT;
}
