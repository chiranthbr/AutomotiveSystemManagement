#include "iostream"
#include "realTimeData.h"
#include "sqlite3.h"
#include "create.h"
#include "obdInterface.h"
#include <string>
#include "alertManagement.h"

using namespace std;

int realTimeData::insertData(int vehicleId, string timestamp) {

   MockOBDInterface* obd = new MockOBDInterface();

   int engine = obd -> getEngineSpeed();
   int vehicle = obd -> getVehicleSpeed();
   int temperature = obd -> getCoolantTemperatue();
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

   int rc = sqlite3_open("../database/vehicleDatase.sqlite", &database);

   if(rc) {
      cout << "Couldnt open database" << endl;
      return 1;
   } else {
      cout << "Opened database successfully" << endl;
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
      sqlite3_free(errMsg);
   } else {
      cout << "Data inserted to dtc successffully" << endl;
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

   std::string temp = "delete from RealTimeData where ID = " + std::to_string(id) + ";";

   const char* sql = temp.c_str();

   rc = sqlite3_exec(database, sql, callback, 0, &errMsg);

   if(rc != SQLITE_OK) {
      cout << "Couldnt delete: " << errMsg << endl;
      sqlite3_free(errMsg);
   } else {
      if(sqlite3_changes(database) == 0) {
         cout << "No matching row found (It might already be deleted or may not have been inserted!!)" << endl;
         return;
      }
      cout << "Data deleted successsfully!!" << errMsg << endl;
   }
}
