#ifndef REAL_TIME_DATA_MANAGEMENT
#define REAL_TIME_DATA_MANAGEMENT

#include <string>
#include "sqlite3.h"


class realTimeDataManagement {
   public: 
      int insertData(int vehicleId, std::string timestamp);
      void del(int id);
      void showMenu();
};

#endif // !REAL_TIME_DATA_MANAGEMENT
