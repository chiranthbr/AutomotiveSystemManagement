#include "iostream"
#include "vehicleManagement.h"
#include "sqlite3.h"
#include <algorithm>
#include <string>
#include "create.h"
#include "vector"

using namespace std;


void VehicleManagement::showColumns() {

   int rc = sqlite3_open("../database/vehicleDatase.sqlite", &database);
   if(rc) {
      cout << "Couldnt open database" << endl;
      return;
   }

   char* errMsg = 0;
   std::string getColumns = "select * from vehicles;";
   const char* getcols = getColumns.c_str();

   rc = sqlite3_exec(database, getcols, callbackGetColumns, 0, &errMsg);
   cout << "Press <Enter> if dont want to change else enter the value to change it into: " << endl;

}

int VehicleManagement::insertData(string make, string model, int year, string vin, string fuelType) {
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

void VehicleManagement::del(int id) {
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



void VehicleManagement::update(std::string make, std::string model, int year, std::string vin, std::string fuelType, int id) {

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

void VehicleManagement::showMenu() {
   int choice;
   std::string menu = "1. Insert \n2. Update \n3. Delete \n4. Back";
   cout << menu;
   cin >> choice;

   switch(choice) {
      case 1:
         std::string make, model, vin, fuel;
         int year;
         cout << "Make: ";
         cin >> make;
         cout << "Model: ";
         cin >> model;
         cout << "Year: ";
         cin >>  year;
         cout << "VIN: ";
         cin >> vin;
         cout << "Fuel Type: ";
         cin >>fuel;
         this -> insertData(make, model, year, vin, fuel);
         break;
      case 3:
         int id;
         cout << "Enter id to be deleted: ";
         cin >> id;
         this -> del(id);
         break;
      case 2:
         this -> showColumns();
         int rc = sqlite3_open("../database/vehicleDatase.sqlite", &database);
         std::string make, model, vin, fuel;
         int year, id;
         cout << "Make: ";
         cin >> make;
         cout << "Model: ";
         cin >> model;
         cout << "Year: ";
         cin >>  year;
         cout << "VIN: ";
         cin >> vin;
         cout << "Fuel Type: ";
         cin >>fuel;
         cout << "ID  (this is compulsory): ";
         cin >> id;

         vector<std::string> s;
         int rc;
         std::string sql = "select Make, Model, year, VIN, FuelType from Vehicles where ID = " + std::to_string(id) + ";";
         char* errMsg = 0;
         sqlite3_stmt* stmt;
         rc = sqlite3_prepare_v2(database, sql.c_str(), -1, &stmt, 0);
         if(rc != SQLITE_OK) {
            cout << sqlite3_errmsg(database) << endl;
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
            return;
         } else {
            cout << "Sql error" << sqlite3_errmsg(database) << endl;
            sqlite3_finalize(stmt);
            sqlite3_close(database);
            return;
         }
         
         sqlite3_finalize(stmt);
         sqlite3_close(database);

         if(make == "") make = s[0];
         if(model == "") model = s[1];
         if(year == "") year = atoi(s[2]);
         else year = atoi(year);
         if(vin == "") vin = s[3];
         if(fuel == "") fuel = s[4];

         this -> update(make, model, year, vin, fuel, id);

   }
}
