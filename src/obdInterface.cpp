#include "iostream"
#include "obdInterface.h"
#include <cstdlib>
#include <ctime>

using namespace std;

int MockOBDInterface::getEngineSpeed() {
   std::srand(std::time(0));

   int speed = std::rand() % (6001);
   return speed;
}
int MockOBDInterface::getVehicleSpeed() {
   std::srand(std::time(0));

   int speed = std::rand() % (301);
   return speed;
}
int MockOBDInterface::getCoolantTemperature() {
   std::srand(std::time(0));

   int temperature = 150 + std::rand() % (101);
   return temperature;
}
int MockOBDInterface::getFuelLevel() {
   std::srand(std::time(0));

   int level = std::rand() % (101);
   return level;
}
