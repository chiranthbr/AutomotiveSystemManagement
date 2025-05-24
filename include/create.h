#ifndef CREATE
#define CREATE

#include "sqlite3.h"

extern sqlite3* database;

int callbackGetColumns(void* NotUsed, int argc, char** argv, char** azColName);
int callback(void *NotUsed, int argc, char **argv, char **azColName);
int createDbAndTables();
void closeDb();

#endif // !DEBUG
