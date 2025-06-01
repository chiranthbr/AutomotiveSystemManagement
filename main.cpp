#include <sqlite3.h>
#include <create.h>
#include "include/create.h"
#include "include/realTimeData.h"
#include "vehicleManagement.h"
#include "DTCManagement.h"
#include "dataAnalyze.h"
#include "realTimeData.h"
#include "iostream"

using namespace std;

 
int main() {

   VehicleManagement* vehicle = new VehicleManagement();
   DtcManagement* dtc = new DtcManagement();
   realTimeDataManagement* realTime = new realTimeDataManagement();  
   DataAnalysisManager* data = new DataAnalysisManager();

   std::string menu = "1. Vehicles\n2. DTC \n3. Realtimedata \n4. Data analysis\n";
   cout << menu;

   int choice;
   cin >> choice;

   switch(choice) {
      case 1:
         {
            vehicle -> showMenu();
            break;
         }
      case 2:
         {
            dtc -> showMenu();
            break;
         }
      case 3:
         {
            realTime -> showMenu();
         }
      case 4:
         {
            data -> analyzeVehiclePerformance();
         }
   }

   closeDb();
   return 0;
}



