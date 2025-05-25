#include "DTCManagement.h"
#include "alertManagement.h"
#include "iostream"
#include "create.h"
#include "sqlite3.h"
#include <string>
#include <ctime>
#include <vector>
#include <limits>

using namespace std;

void DtcManagement::showColumns() {
 
   int rc = sqlite3_open("../database/automotiveDatabase.sqlite", &database);
   if(rc) {
      cout << "Couldnt open database: " << sqlite3_errmsg(database) << endl;
   }

   char* errMsg = 0;
   std::string getColumns = "select * from DTCs;";
   const char* getcols = getColumns.c_str();

   rc = sqlite3_exec(database, getcols, callbackGetColumns, 0, &errMsg);
   sqlite3_close(database);
}


int DtcManagement::insertData(int vehicleId, string DTCCode, string description, string severity) {


   int rc = sqlite3_open("../database/automotiveDatabase.sqlite", &database);
   if(rc) {
      cout << "Couldnt open database: " << sqlite3_errmsg(database) << endl;
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
      time_t timestamp;
      time(&timestamp);
      AlertManagement* alert = new AlertManagement();
      alert -> insertData(vehicleId, ctime(&timestamp), DTCCode, description);
      sqlite3_close(database);
   }
   return 0;
}

void DtcManagement::del(int id) {


   int rc = sqlite3_open("../database/automotiveDatabase.sqlite", &database);
   if(rc) {
      cout << "Couldnt open database: " << sqlite3_errmsg(database) << endl;
   }

   char* errMsg = 0;

   std::string temp = "delete from DTCs where ID = " + std::to_string(id) + ";";

   const char* sql = temp.c_str();

   rc = sqlite3_exec(database, sql, callback, 0, &errMsg);

   if(rc != SQLITE_OK) {
      cout << "Coulnt delete: " << errMsg << endl;
      sqlite3_free(errMsg);
      sqlite3_close(database);
   } else {
      if(sqlite3_changes(database) == 0) {
         cout << "No matching row found (It might already be deleted or may not have been inserted!!)" << endl;
         sqlite3_close(database);
         return;
      }
      cout << "Data deleted successsfully!!" << errMsg << endl;
      sqlite3_close(database);
   }

}


void DtcManagement::update(std::string DTCCode, std::string description, std::string severity, int id) {


   int rc = sqlite3_open("../database/automotiveDatabase.sqlite", &database);
   if(rc) {
      cout << "Couldnt open database: " << sqlite3_errmsg(database) << endl;
   }

   std::string sql = "update DTCs set DTCCode = ?, description = ?, severity = ? where ID = ?";

   sqlite3_stmt* stmt;
   if(sqlite3_prepare_v2(database, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
      cout << "Couldnt update: " << sqlite3_errmsg(database) << endl;
      sqlite3_close(database);
      return;
   }

   sqlite3_bind_text(stmt, 1, DTCCode.c_str(), -1, SQLITE_STATIC);
   sqlite3_bind_text(stmt, 2, description.c_str(), -1, SQLITE_STATIC);
   sqlite3_bind_text(stmt, 3, severity.c_str(), -1, SQLITE_STATIC);
   sqlite3_bind_int(stmt, 4, id);

   if(sqlite3_step(stmt) != SQLITE_DONE) {
      cout << "Couldnt update: " << sqlite3_errmsg(database) << endl;
      sqlite3_close(database);
      sqlite3_finalize(stmt);
      return;
   } else {
      cout << "Updated successfully!!" << endl;
      sqlite3_close(database);
   }

   sqlite3_finalize(stmt);
}

void DtcManagement::showMenu() {
   int choice;
   std::string menu = "1. Insert \n2. Update \n3. Delete \n4. Back\n";
   cout << menu;
   cin >> choice;

   std::string dtcCode, vehicleId, description, severity, id;

   switch(choice) {
      case 1:
      {
         cout << "DTC code: ";
         cin >> dtcCode;
         cout << "Description: ";
         cin >> description;
         cout << "severity: ";
         cin >>  severity;
         cout << "Vehicle id: ";
         cin >> vehicleId;

         this -> insertData(stoi(vehicleId), dtcCode, description, severity);
         break;
      }
      case 2:
      {
         std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Clear leftover newline

         cout << "DTC code: " << endl;
         std::getline(std::cin, dtcCode);
         cout << "Description: ";
         std::getline(std::cin, description);
         cout << "Severity: ";
         std::getline(std::cin, severity);
         
         cout << "ID  (this is compulsory): ";
         std::getline(std::cin, id);

         vector<std::string> s;
         std::string sql = "select DTCCode, Description, Severity from Vehicles where ID = " + id + ";";

         int rc = sqlite3_open("../database/automotiveDatabase.sqlite", &database);
         if(rc) {
            cout << "Couldnt open database: " << sqlite3_errmsg(database) << endl;
         }
         char* errMsg = 0;
         sqlite3_stmt* stmt;
         rc = sqlite3_prepare_v2(database, sql.c_str(), -1, &stmt, 0);
         if(rc != SQLITE_OK) {
            cout << sqlite3_errmsg(database) << endl;
            sqlite3_close(database);
            return;
         }
         rc = sqlite3_step(stmt);
         if(rc == SQLITE_ROW) {
            int columnCount = sqlite3_column_count(stmt);
            for(int i = 0; i < columnCount; i++) {
               const unsigned char* data = sqlite3_column_text(stmt, i);
               if(data) {
                  s.push_back(std::string((const char*)data));
               }
            }
         } else if(rc == SQLITE_DONE) {
            cout << "NO matching row found: " << endl;
            sqlite3_close(database);
            return;
         } else {
            cout << "Sql error" << sqlite3_errmsg(database) << endl;
            sqlite3_finalize(stmt);
            sqlite3_close(database);
            return;
         }
         
         sqlite3_finalize(stmt);

         if(dtcCode == "") dtcCode = s[0];
         if(description == "") description = s[1];
         if(severity == "") severity = s[2];
         sqlite3_close(database);

         this -> update(dtcCode, description, severity, stoi(id));
         break;
      }
      case 3:
      {
         int id;
         cout << "Enter id to be deleted: ";
         cin >> id;
         this -> del(id);
         break;
      }
      default:
      {
         cout << "Enter correct choice" << endl;
         this -> showMenu();
      }
   }
}
