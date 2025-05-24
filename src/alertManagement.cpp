#include "iostream"
#include "create.h"
#include "alertManagement.h"
#include <string>
#include "sqlite3.h"

using namespace std;

int AlertManagement::insertData(int vehicleId, string timestamp, string alertType, string description) {
   int rc = sqlite3_open("../database/vehicleDatase.sqlite", &database);

   if(rc) {
      cout << "Couldnt open database" << endl;
      return 1;
   } else {
      cout << "Opened database successfully" << endl;
   }

   char* errMsg = 0;

   std::string temp = "insert into Alerts(VehicleId, Timestamp, AlertType, Description) values ('"\
                      + std::to_string(vehicleId) + "', '"\
                      + timestamp + "', '"\
                      + alertType + "', '"\
                      + description + "');";

   const char* sql = temp.c_str();
   rc = sqlite3_exec(database, sql, callback, 0, &errMsg);

   if(rc != SQLITE_OK) {
      cout << "Cant insert: " << errMsg << endl;
      sqlite3_free(errMsg);
   } else {
      cout << "Data inserted to dtc successffully" << endl;
   }
   return 0;
}
