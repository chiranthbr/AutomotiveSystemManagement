#include "DTCManagement.h"
#include "iostream"
#include "create.h"
#include "sqlite3.h"
#include <string>
#include <ctime>

using namespace std;

void DtcManagement::showColumns() {

   int rc = sqlite3_open("../database/vehicleDatase.sqlite", &database);
   if(rc) {
      cout << "Couldnt open database" << endl;
      return;
   }

   char* errMsg = 0;
   std::string getColumns = "select * from DTCs;";
   const char* getcols = getColumns.c_str();

   rc = sqlite3_exec(database, getcols, callbackGetColumns, 0, &errMsg);
}

int DtcManagement::insertData(int vehicleId, string DTCCode, string description, string severity) {
   int rc = sqlite3_open("../database/vehicleDatase.sqlite", &database);

   if(rc) {
      cout << "Couldnt open database" << endl;
      return 1;
   } else {
      cout << "Opened database successfully" << endl;
   }

   char* errMsg = 0;

   std::string temp = "insert into DTCs(VehicleId, DTCCode, Description, Severity) values ('"\
                      + std::to_string(vehicleId) + "', '"\
                      + DTCCode + "', '"\
                      + description + "', '"\
                      + severity + "');";

   const char* sql = temp.c_str();
   rc = sqlite3_exec(database, sql, callback, 0, &errMsg);

   if(rc != SQLITE_OK) {
      cout << "Cant insert: " << errMsg << endl;
      sqlite3_free(errMsg);
   } else {
      cout << "Data inserted to dtc successffully" << endl;
      time_t = timestamp;
      time(&timestamp);
      alert -> insertData(vehicleId, ctime(&timestamp), DTCCode, description);
   }
   return 0;
}

void DtcManagement::del(int id) {
   int rc = sqlite3_open("../database/vehicleDatase.sqlite", &database);
   if(rc) {
      cout << "Couldnt open database." << endl;
      return;
   }

   char* errMsg = 0;

   std::string temp = "delete from DTCs where ID = " + std::to_string(id) + ";";

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


void DtcManagement::update(std::string DTCCode, std::string description, std::string severity, int id) {

   showColumns();

   std::string sql = "update DTCs set DTCCode = ?, description = ?, severity = ? where ID = ?";

   sqlite3_stmt* stmt;
   if(sqlite3_prepare_v2(database, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
      cout << "Couldnt update: " << sqlite3_errmsg(database) << endl;
      return;
   }

   sqlite3_bind_text(stmt, 1, DTCCode.c_str(), -1, SQLITE_STATIC);
   sqlite3_bind_text(stmt, 2, description.c_str(), -1, SQLITE_STATIC);
   sqlite3_bind_text(stmt, 3, severity.c_str(), -1, SQLITE_STATIC);
   sqlite3_bind_int(stmt, 4, id);

   if(sqlite3_step(stmt) != SQLITE_DONE) {
      cout << "Couldnt update: " << sqlite3_errmsg(database) << endl;
      sqlite3_finalize(stmt);
      return;
   } else {
      cout << "Updated successfully!!" << endl;
   }

   sqlite3_finalize(stmt);
}
