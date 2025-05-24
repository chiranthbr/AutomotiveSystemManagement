#ifndef VEHICLE_MANAGEMENT
#define VEHICLE_MANAGEMENT


#include <string>

class VehicleManagement {
   public:
      void showMenu();
      int insertData(std::string make, std::string model, int year, std::string vin, std::string fuelType);
      void update(std::string make, std::string model, int year, std::string vin, std::string fuelType, int id);
      void del(int id);
      void showColumns();

};



#endif // !DEBUG
       //
       //
       //
