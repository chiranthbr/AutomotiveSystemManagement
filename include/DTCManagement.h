#ifndef DTC_MANAGEMENT
#define DTC_MANAGEMENT

#include <string>

class DtcManagement {
   public:
      int insertData(int vehicleId, std::string DTCCode, std::string description, std::string severity);
      void update(std::string DTCCode, std::string description, std::string severity, int id);
      void del(int id);
      void showColumns();

};



#endif // !DEBUG
