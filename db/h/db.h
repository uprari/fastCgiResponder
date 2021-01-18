#include <mysql.h>
#include <stdbool.h>
#include <errCodes.h>
#define MAX_LINE_SIZE 100
#define ZERO 0
#define ONE 1
#define USER "user"
#define SERVER "server"
#define PASSWORD "password"
#define DATABASE "database"

typedef struct globalDbInfo {

    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
} globalDbInfo;

typedef struct globalDbConfig {

    char *server;
    char *user;
    char *password;
    char *database;

} globalDbConfig;
//global data structures
globalDbInfo dbInfo;
globalDbConfig dbConfig;

// function declarations
tErrCode dbInitialise(void);
tErrCode dbReadConfig(void);
void addConfigValue(char *buffer, int key, int val);
void printGlobalConfig(void);
tErrCode dbConnect(void);
tErrCode dbConfigValidate();
tErrCode thDbConnect(MYSQL **conn);
void dbClean(MYSQL *conn);
