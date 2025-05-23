#include <sqlite3.h>
#include <create.h>
#include "vehicleManagement.h"

using namespace std;
 
int main() {
   createDbAndTables();
   insertData("Tesla", "V2", 2000, "Vin1234", "Diesel");
//   del(3);
   showColumns();
   update("Tesla", "V3", 2019, "Vin1234", "Diesel", 4);
   closeDb();

   return 0;
}
