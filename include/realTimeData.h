#ifndef REAL_TIME_DATA_MANAGEMENT
#define REAL_TIME_DATA_MANAGEMENT

#include <string>


class realTimeDataManagement {
   public: 
      int insertData(int vehicleId, std::string timestamp);
      void del(int id);
};

#endif // !REAL_TIME_DATA_MANAGEMENT
