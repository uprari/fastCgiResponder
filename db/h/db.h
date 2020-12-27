#include <mysql.h>
#include <stdbool.h>
#define MAX_LINE_SIZE 100
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
int escapeSpace(char *buffer, int index);
bool extractKeyVal(char *buf, int *k, int *v , int index);
void addConfigValue(char *buffer, int key, int val);
void printGlobalConfig(void);
