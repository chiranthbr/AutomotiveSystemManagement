#include <string>
#include <vector>
#include "sqlite3.h"
#include <map>

using namespace std;

class DataAnalysisManager {
   public:
      void analyzeVehiclePerformance();

   private:
      struct AvgSpeedData {
        std::map<int, double> vehicleTotalSpeed;
        std::map<int, int> vehicleRecordCount;
      };
      friend int avgSpeedCallback(void* data, int argc, char** argv, char** azColName);
};
