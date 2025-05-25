#include "iostream"
#include "vehicleManagement.h"
#include "sqlite3.h"
#include <algorithm>
#include <cstdlib>
#include <string>
#include "create.h"
#include "vector"
#include <limits>

using namespace std;


void VehicleManagement::showColumns() {


   int rc = sqlite3_open("../database/automotiveDatabase.sqlite", &database);
   if(rc) {
      cout << "Couldnt open database: " << sqlite3_errmsg(database) << endl;
   }

   char* errMsg = 0;
   std::string getColumns = "select * from vehicles;";
   const char* getcols = getColumns.c_str();

   rc = sqlite3_exec(database, getcols, callbackGetColumns, 0, &errMsg);
   cout << "Press <Enter> if dont want to change else enter the value to change it into: " << endl;
   sqlite3_close(database);

}

int VehicleManagement::insertData(string make, string model, int year, string vin, string fuelType) {


   int rc = sqlite3_open("../database/automotiveDatabase.sqlite", &database);
   if(rc) {
      cout << "Couldnt open database: " << sqlite3_errmsg(database) << endl;
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
      cout << "Cant insert: " << sqlite3_errmsg(database) << endl;
      sqlite3_close(database);
      sqlite3_free(errMsg);
   } else {
      cout << "Data inserted successffully" << endl;
      sqlite3_close(database);
   }
   return 0;
}

void VehicleManagement::del(int id) {


   int rc = sqlite3_open("../database/automotiveDatabase.sqlite", &database);
   if(rc) {
      cout << "Couldnt open database: " << sqlite3_errmsg(database) << endl;
   }

   char* errMsg = 0;

   std::string temp = "delete from Vehicles where ID = " + std::to_string(id) + ";";

   const char* sql = temp.c_str();

   rc = sqlite3_exec(database, sql, callback, 0, &errMsg);

   if(rc != SQLITE_OK) {
      cout << "Coulnt delete: " << sqlite3_errmsg(database) << endl;
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


void VehicleManagement::update(std::string make, std::string model, int year, std::string vin, std::string fuelType, int id) {


   int rc = sqlite3_open("../database/automotiveDatabase.sqlite", &database);
   if(rc) {
      cout << "Couldnt open database: " << sqlite3_errmsg(database) << endl;
   }


   std::string sql = "update Vehicles set Make = ?, Model = ?, Year = ?, VIN = ?, FuelType = ? where ID = ?;";

   sqlite3_stmt* stmt;
   if(sqlite3_prepare_v2(database, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
      cout << "Couldnt update: after prepare" << sqlite3_errmsg(database) << endl;
      return;
   }

   sqlite3_bind_text(stmt, 1, make.c_str(), -1, SQLITE_STATIC);
   sqlite3_bind_text(stmt, 2, model.c_str(), -1, SQLITE_STATIC);
   sqlite3_bind_int(stmt, 3, year);
   sqlite3_bind_text(stmt, 4, vin.c_str(), -1, SQLITE_STATIC);
   sqlite3_bind_text(stmt, 5, fuelType.c_str(), -1, SQLITE_STATIC);
   sqlite3_bind_int(stmt, 6, id);

   if(sqlite3_step(stmt) != SQLITE_DONE) {
      cout << "Couldnt update: after bind " << sqlite3_errmsg(database) << endl;
      sqlite3_close(database);
      return;
   } else {
      cout << "Updated successfully!!" << endl;
      sqlite3_close(database);
   }

   sqlite3_finalize(stmt);
}

void VehicleManagement::showMenu() {
   
   int choice;
   std::string menu = "1. Insert \n2. Update \n3. Delete \n4. Back\n";
   cout << menu;
   cin >> choice;

   std::string make, model, vin, fuel, year, id;

   switch(choice) {
      case 1:
      {
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
         this -> insertData(make, model, stoi(year), vin, fuel);
         break;
      }
      case 2:
      {
         std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Clear leftover newline

         cout << "Make: " << endl;
         std::getline(std::cin, make);
         cout << "Model: ";
         std::getline(std::cin, model);
         cout << "Year: ";
         std::getline(std::cin, year);
         cout << "VIN: ";
         std::getline(std::cin, vin);
         cout << "Fuel Type: ";
         std::getline(std::cin, fuel);
         cout << "ID  (this is compulsory): ";
         std::getline(std::cin, id);

         vector<std::string> s;
         std::string sql = "select Make, Model, year, VIN, FuelType from Vehicles where ID = " + id + ";";

         int rc = sqlite3_open("../database/automotiveDatabase.sqlite", &database);
         if(rc) {
            cout << "Couldnt open database; " << sqlite3_errmsg(database) << endl;
         }
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
         if(year == "") year = s[2];
         if(vin == "") vin = s[3];
         if(fuel == "") fuel = s[4];

         this -> update(make, model, stoi(year), vin, fuel, stoi(id));
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
