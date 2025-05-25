#include "create.h"
#include "iostream"
#include "sqlite3.h"
#include <string.h>
#include "vector"
#include <string>
#include <typeinfo>

extern sqlite3* database = nullptr;

using namespace std;

int callbackGetColumns(void* Notused, int argc, char** argv, char** azColName) {
   for(int i = 0; i < argc; i++) {
      cout << i + 1 << " : " << azColName[i] << endl;
   }
   cout << "enter numbers with spaces or enter to update values: (enter any character to stop, except space and tab)" << endl;

   return 0;
}

int callback(void *NotUsed, int argc, char **argv, char **azColName) {
   int i;
   string temp = argv[i] ? argv[i] : "NULL";
   for(i = 0; i<argc; i++) {
      std::cout << azColName[i] << " = " << temp << std::endl;
   }
   std::cout << std::endl;
   return 0;
}
int createDbAndTables() {
   
   int rc = sqlite3_open("../database/automotiveDatabase.sqlite", &database);
   if(rc) {
      cout << "Couldnt open database: " << sqlite3_errmsg(database) << endl;
   }

   char* errMsg = 0;

   //Vehicles table
   const char* sql = "CREATE TABLE IF NOT EXISTS Vehicles("  \
                     "ID INTEGER PRIMARY KEY AUTOINCREMENT," \
                     "Make text,"\
                     "Model text,"\
                     "Year integer,"\
                     "VIN text unique,"\
                     "FuelType text);";

   rc = sqlite3_exec(database, sql, callback, 0, &errMsg);
   
   if(rc != SQLITE_OK) {
      std::cout << "Couldnt create table vehicles: " << sqlite3_errmsg(database) << std::endl;
   }

   sql = "CREATE TABLE IF NOT EXISTS DTCs("  \
                     "ID INTEGER PRIMARY KEY AUTOINCREMENT," \
                     "VehicleId integer,"\
                     "DTCCode text,"\
                     "Description text,"\
                     "Severity text,"\
                     "foreign key(VehicleId) references Vehicles(ID));";

   rc = sqlite3_exec(database, sql, callback, 0, &errMsg);

   if(rc != SQLITE_OK) {
      std::cout << "Couldnt create table: " << errMsg << std::endl;
   }

   sql = "CREATE TABLE IF NOT EXISTS RealTimeData("  \
                     "ID INTEGER PRIMARY KEY AUTOINCREMENT,"\
                     "VehicleID INTEGER,"\
                     "Timestamp DATETIME,"\
                     "EngineSpeed INTEGER,"\
                     "VehicleSpeed INTEGER,"\
                     "CoolantTemperature INTEGER,"\
                     "FuelLevel REAL,"\
                     "FOREIGN KEY (VehicleID) REFERENCES Vehicles(ID));";

   rc = sqlite3_exec(database, sql, callback, 0, &errMsg);

   if(rc != SQLITE_OK) {
      std::cout << "Couldnt create table real time: " << errMsg << std::endl;
   }
 
   sql = "CREATE TABLE IF NOT EXISTS Alerts ("  \
                     "ID INTEGER PRIMARY KEY AUTOINCREMENT," \
                     "VehicleId integer,"\
                     "Timestamp datetime,"\
                     "AlertType text,"\
                     "Description text,"\
                     "foreign key(VehicleId) references Vehicles(ID));";

   rc = sqlite3_exec(database, sql, callback, 0, &errMsg);

   if(rc != SQLITE_OK) {
      std::cout << "Couldnt create table alert: " << errMsg << std::endl;
   }

   sql = "CREATE TABLE IF NOT EXISTS Users ("\
          "ID integer primary key autoincrement,"\
          "Username text unique,"\
          "Password text,"\
          "role text);";

   rc = sqlite3_exec(database, sql, callback, 0, &errMsg);

   if(rc != SQLITE_OK) {
      std::cout << "Couldnt create table users: " << errMsg << std::endl;
   }

   sqlite3_close(database);

   return 0;
}

void closeDb() {
   sqlite3_close(database);
}
