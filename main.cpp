#include <sqlite3.h>
#include <create.h>
#include "include/create.h"
#include "vehicleManagement.h"
#include "DTCManagement.h"

using namespace std;

 
int main() {

   VehicleManagement* vehicle = new VehicleManagement();
   DtcManagement* dtc = new DtcManagement();
   createDbAndTables();
   vehicle -> showMenu();
   closeDb();
   return 0;
}



