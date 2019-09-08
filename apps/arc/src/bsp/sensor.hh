#ifndef ARC_SENSOR_HH
#define ARC_SENSOR_HH

#include "comm_link_interface.hh"
#include "sensor_ST_LSM6DS33_defs.hh"

namespace arc {
  namespace arc {
    namespace bsp {

/** Generic sensor class
 */
class Sensor {
 public:
  // Sensor();

  virtual void Query() = 0;
};

/**
 * Implements interface to LSM6DS33 6-DOF IMU
 */
class ST_6DOFImu_LSM6DS33 : public Sensor, public SPIInterface {
 public:
  ST_6DOFImu_LSM6DS33();
  ~ST_6DOFImu_LSM6DS33();

  void Query() override;
  float GetTemperature();

 private:
  using RegisterMap = st_lsm6ds33_defs::RegisterMap;
  using DataRegisters = st_lsm6ds33_defs::DataRegisters;

  inline uint8_t MakeCommandByte(RegisterMap reg_addr, bool do_read) {
    return ((uint8_t) reg_addr) | ((do_read & 0x01) << 7);
  };

  uint8_t ReadAddress(RegisterMap reg_addr);

  /* Variables */
  static const uint8_t kIDCode = 0x69;
  DataRegisters raw_data;
};

/**
 * Implements interface to LIS3MDL 3-DOF Magnetometer
 */
class ST_3DOFMag_LIS3MDL : public Sensor, public SPIInterface {
 public:
  ST_3DOFMag_LIS3MDL();
  ~ST_3DOFMag_LIS3MDL();

  void Query() override;
};

    } // arc::arc::bsp
  }
}

#endif // ARC_SENSOR_HH