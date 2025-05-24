#include <sqlite3.h>
#include <create.h>
#include "vehicleManagement.h"
#include "DTCManagement.h"

using namespace std;
 
int main() {
   VehicleManagement* vehicle = new VehicleManagement();
   DtcManagement* dtc = new DtcManagement();
   createDbAndTables();
   vehicle -> insertData("Tesla", "V2", 2000, "Vin1234", "Diesel");
//   del(3);
   vehicle -> update("Tesla", "V3", 2019, "Vin1234", "Diesel", 4);
   dtc -> insertData(4, "DTC2025yafajlfj", "Issue in dashboard updation", "Very low");

   dtc -> update("DTC2025fjajl", "Issue in dashboard updation", "High", 1);
   closeDb();

   return 0;
}



