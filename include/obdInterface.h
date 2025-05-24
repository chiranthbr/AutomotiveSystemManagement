#ifndef OBD_PROTOCOL_INTERFACE
#define OBD_PROTOCOL_INTERFACE

class OBDInterface {
   public:
      virtual int getEngineSpeed() = 0;
      virtual int getVehicleSpeed() = 0;
      virtual int getCoolantTemperature() = 0;
      virtual int getFuelLevel() = 0;
      virtual ~OBDInterface() = default;
};

class MockOBDInterface: public OBDInterface {
   public:
      int getEngineSpeed();
      int getVehicleSpeed();
      int getCoolantTemperature();
      int getFuelLevel();
};

#endif // !OBD_PROTOCOL_INTERFACE

