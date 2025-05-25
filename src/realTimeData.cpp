#include "iostream"
#include "realTimeData.h"
#include "sqlite3.h"
#include "create.h"
#include "obdInterface.h"
#include <string>
#include "alertManagement.h"
#include <ctime>

using namespace std;

int realTimeDataManagement::insertData(int vehicleId, string timestamp) {

   MockOBDInterface* obd = new MockOBDInterface();

   int engine = obd -> getEngineSpeed();
   int vehicle = obd -> getVehicleSpeed();
   int temperature = obd -> getCoolantTemperature();
   int fuelLevel = obd -> getFuelLevel();

   AlertManagement* alert = new AlertManagement();

   string alertType = "";
   if(engine > 5000) {
      alertType = "High RPM";
      alert -> insertData(vehicleId, timestamp, alertType, "Going at High than maximum safest RPM");

   }
   if(vehicle > 280) {
      alertType = "Over Speed";
      alert -> insertData(vehicleId, timestamp, alertType, "Vehicle is overspeeding");
   }
   if(temperature > 120) {
      alertType = "Over heat";
      alert -> insertData(vehicleId, timestamp, alertType, "Engine coolant temperature is too high");
   }
   if(fuelLevel < 10) {
      alertType = "Low fuel";
      alert -> insertData(vehicleId, timestamp, alertType, "Fuel level is critically low");
   }

   int rc = sqlite3_open("../database/automotiveDatabase.sqlite", &database);
   if(rc) {
      cout << "Couldnt open database: " << sqlite3_errmsg(database) << endl;
   }



   char* errMsg = 0;

   std::string temp = "insert into RealTimeData(VehicleId, Timestamp, EngineSpeed, VehicleSpeed, CoolantTemperature, FuelLevel) values ('"\
                      + std::to_string(vehicleId) + "', '"\
                      + timestamp + "', '"\
                      + std::to_string(engine) + "', '"\
                      + std::to_string(vehicle) + "', '"\
                      + std::to_string(temperature) + "', '"\
                      + std::to_string(fuelLevel) + "');";

   const char* sql = temp.c_str();
   rc = sqlite3_exec(database, sql, callback, 0, &errMsg);

   if(rc != SQLITE_OK) {
      cout << "Cant insert: " << errMsg << endl;
      sqlite3_close(database);
      sqlite3_free(errMsg);
   } else {
      cout << "Data inserted to dtc successffully" << endl;
      sqlite3_close(database);
   }
   return 0;
}


void realTimeDataManagement::del(int id) {


   int rc = sqlite3_open("../database/automotiveDatabase.sqlite", &database);
   if(rc) {
      cout << "Couldnt open database: " << sqlite3_errmsg(database) << endl;
   }

   char* errMsg = 0;

   std::string temp = "delete from RealTimeData where ID = " + std::to_string(id) + ";";

   const char* sql = temp.c_str();

   rc = sqlite3_exec(database, sql, callback, 0, &errMsg);

   if(rc != SQLITE_OK) {
      cout << "Couldnt delete: " << errMsg << endl;
      sqlite3_close(database);
      sqlite3_free(errMsg);
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

void realTimeDataManagement::showMenu() {
   std::string menu = "1. Insert \n2. Delete";
   cout << menu;

   int choice;
   cin >> choice;

   switch(choice) {
      case 1:
         {
            int vehicleId;
            time_t timestamp;
            time(&timestamp);
            cout << "Enter vehicle id to insert: ";
            cin >> vehicleId;
            this -> insertData(vehicleId, ctime(&timestamp));
            break;
         }
      case 2:
         {
            int id;
            cout << "Enter ID to delete: ";
            cin >> id;
            this -> del(id);
            break;
         }
      default:
         {
            cout << "Enter correct choice;" << endl;
            this -> showMenu();
         }
   }
}
