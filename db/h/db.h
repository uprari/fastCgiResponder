#include <mysql.h>

typedef struct globalDbInfo{

	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
} globalDbInfo;

typedef struct globalDbConfig{

	char *server;
	char *user;
	char *password;
	char *database;

} globalDbConfig;

// function declarations
void dbInitialise(void);
void dbReadConfig(void);
