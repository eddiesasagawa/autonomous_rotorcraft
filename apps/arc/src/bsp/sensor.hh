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

  double time_valid;
  float temperature;
  float f_imx_m;     // specific force x-dir from accelerometer in sensor frame
  float f_imy_m;     // specific force y-dir from accelerometer in sensor frame
  float f_imz_m;     // specific force z-dir from accelerometer in sensor frame
  float omega_imx_m; // angular rate x-dir from gyroscope in sensor frame
  float omega_imy_m; // angular rate y-dir from gyroscope in sensor frame
  float omega_imz_m; // angular rate z-dir from gyroscope in sensor frame

 private:
  using RegisterMap = st_lsm6ds33_defs::RegisterMap;
  using TimeRegisters = st_lsm6ds33_defs::TimeRegisters;
  using DataRegisters = st_lsm6ds33_defs::DataRegisters;
  using CtrlRegisters = st_lsm6ds33_defs::CtrlRegisters;

  inline uint8_t MakeCommandByte(RegisterMap reg_addr, bool do_read) {
    return ((uint8_t) reg_addr) | ((do_read & 0x01) << 7);
  };

  template<typename FP>
  inline FP DecodeInt16(int16_t in_val, float lsb) {
    return ((FP)in_val) * lsb;
  }

  uint8_t       ReadRegister(RegisterMap reg_addr);
  void          WriteRegister(RegisterMap reg_addr, uint8_t new_val);
  CtrlRegisters GrabCtrlSettings();
  void          LoadCtrlSettings(CtrlRegisters new_settings);
  void          GrabData();

  void          PrintByteArray(const uint8_t* const data, uint16_t num_bytes, uint16_t offset, uint8_t start_addr);

  /* Variables */
  static const uint8_t kIDCode = 0x69;
  float time_us_lsb_;
  TimeRegisters raw_time_;
  DataRegisters raw_data_;
  CtrlRegisters ctrl_settings_;
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