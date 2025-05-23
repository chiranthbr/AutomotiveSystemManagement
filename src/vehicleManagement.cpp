#include "iostream"
#include "vehicleManagement.h"
#include "sqlite3.h"
#include <string>
#include "create.h"

using namespace std;

int callbackGetColumns(void* Notused, int argc, char** argv, char** azColName) {
   for(int i = 0; i < argc; i++) {
      cout << i + 1 << " : " << azColName[i] << endl;
   }
   cout << "enter numbers with spaces or enter to update values: (enter any character to stop, except space and tab)" << endl;

   return 0;
}

void showColumns() {

   int rc = sqlite3_open("../database/vehicleDatase.sqlite", &database);
   if(rc) {
      cout << "Couldnt open database" << endl;
      return;
   }

   char* errMsg = 0;
   std::string getColumns = "select * from vehicles;";
   const char* getcols = getColumns.c_str();

   rc = sqlite3_exec(database, getcols, callbackGetColumns, 0, &errMsg);
}

int insertData(string make, string model, int year, string vin, string fuelType) {
   int rc = sqlite3_open("../database/vehicleDatase.sqlite", &database);

   if(rc) {
      cout << "Couldnt open database" << endl;
      return 1;
   } else {
      cout << "Opened database successfully" << endl;
   }

   char* errMsg = 0;

   std::string temp = "insert into Vehicles(Make, Model, Year, VIN, FuelType) values ('"\
                      + make + "', '"\
                      + model + "', '"\
                      + std::to_string(year) + "', '"\
                      + vin + "', '"\
                      + fuelType + "');";

   const char* sql = temp.c_str();
   rc = sqlite3_exec(database, sql, callback, 0, &errMsg);

   if(rc != SQLITE_OK) {
      cout << "Cant insert: " << errMsg << endl;
      sqlite3_free(errMsg);
   } else {
      cout << "Data inserted successffully" << endl;
   }
   return 0;
}

void del(int id) {
   int rc = sqlite3_open("../database/vehicleDatase.sqlite", &database);
   if(rc) {
      cout << "Couldnt open database." << endl;
      return;
   }

   char* errMsg = 0;

   std::string temp = "delete from Vehicles where ID = " + std::to_string(id) + ";";

   const char* sql = temp.c_str();

   rc = sqlite3_exec(database, sql, callback, 0, &errMsg);

   if(rc != SQLITE_OK) {
      cout << "Coulnt delete: " << errMsg << endl;
      sqlite3_free(errMsg);
   } else {
      if(sqlite3_changes(database) == 0) {
         cout << "No matching row found (It might already be deleted or may not have been inserted!!)" << endl;
         return;
      }
      cout << "Data deleted successsfully!!" << errMsg << endl;
   }
}



void update(std::string make, std::string model, int year, std::string vin, std::string fuelType, int id) {
   std::string sql = "update Vehicles set Make = ?, Model = ?, Year = ?, VIN = ?, FuelType = ? where ID = ?";

   sqlite3_stmt* stmt;
   if(sqlite3_prepare_v2(database, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
      cout << "Couldnt update: " << sqlite3_errmsg(database) << endl;
      return;
   }

   sqlite3_bind_text(stmt, 1, make.c_str(), -1, SQLITE_STATIC);
   sqlite3_bind_text(stmt, 2, model.c_str(), -1, SQLITE_STATIC);
   sqlite3_bind_int(stmt, 3, year);
   sqlite3_bind_text(stmt, 4, vin.c_str(), -1, SQLITE_STATIC);
   sqlite3_bind_text(stmt, 5, fuelType.c_str(), -1, SQLITE_STATIC);
   sqlite3_bind_int(stmt, 6, id);

   if(sqlite3_step(stmt) != SQLITE_DONE) {
      cout << "Couldnt update: " << sqlite3_errmsg(database) << endl;
      return;
   } else {
      cout << "Updated successfully!!" << endl;
   }

   sqlite3_finalize(stmt);
}
