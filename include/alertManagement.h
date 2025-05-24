#ifndef ALERT_MANAGEMENT
#define ALERT_MANAGEMENT

#include <string>

class AlertManagement {
   public:
      int insertData(int vehicleId, std::string timestamp, std::string alertType, std::string description);
};

#endif // !DEBUG
