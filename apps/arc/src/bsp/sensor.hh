#ifndef ARC_SENSOR_HH
#define ARC_SENSOR_HH

#include "comm_link_interface.hh"

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

 private:
  enum RegisterMap {
    kLSM6DS33Addr_WHO_AM_I = 0x0F,  /* read-only; fixed at 0x69 */
    kLSM6DS33Addr_CTRL1_XL  = 0x10,  /* linear accel sensor control register 1 */
    kLSM6DS33Addr_CTRL2_G   = 0x11,
    kLSM6DS33Addr_CTRL3_C   = 0x12,
    kLSM6DS33Addr_CTRL4_C   = 0x13,
    kLSM6DS33Addr_CTRL5_C   = 0x14,
    kLSM6DS33Addr_CTRL6_C   = 0x15,
    kLSM6DS33Addr_CTRL7_G   = 0x16,
    kLSM6DS33Addr_CTRL8_XL  = 0x17,
    kLSM6DS33Addr_CTRL9_XL  = 0x18,
    kLSM6DS33Addr_CTRL10_C  = 0x19,

    kLSM6DS33Addr_STATUS_REG  = 0x1E, /* status register */
    kLSM6DS33Addr_OUT_TEMP_L  = 0x20,
    kLSM6DS33Addr_OUT_TEMP_H  = 0x21, /* temperature sensor output data, 2's complement */
    kLSM6DS33Addr_OUTX_L_G = 0x22,  /* Angular rate sensor - 16-bit word in 2's comp */
    kLSM6DS33Addr_OUTX_H_G = 0x23,
    kLSM6DS33Addr_OUTY_L_G = 0x24,
    kLSM6DS33Addr_OUTY_H_G = 0x25,
    kLSM6DS33Addr_OUTZ_L_G = 0x26,
    kLSM6DS33Addr_OUTZ_H_G = 0x27,
    kLSM6DS33Addr_OUTX_L_XL = 0x28,  /* Linear acceleration output - 16-bit word in 2's comp */
    kLSM6DS33Addr_OUTX_H_XL = 0x29,
    kLSM6DS33Addr_OUTY_L_XL = 0x2A,
    kLSM6DS33Addr_OUTY_H_XL = 0x2B,
    kLSM6DS33Addr_OUTZ_L_XL = 0x2C,
    kLSM6DS33Addr_OUTZ_H_XL = 0x2D
  };

  union CtrlRegisters {
    struct {
      struct {
        uint8_t BW_XL : 2;
        uint8_t FS_XL : 2;
        uint8_t ODR_XL  : 4;
      } ctrl_1;

      struct {
        uint8_t reserved  : 1;
        uint8_t FS_125    : 1;
        uint8_t FS_G      : 2;
        uint8_t ODR_G     : 4;
      } ctrl_2;

      struct {
        uint8_t SW_RESET  : 1;
        uint8_t BLE       : 1;
        uint8_t IF_INC    : 1;
        uint8_t SIM       : 1;
        uint8_t PP_OD     : 1;
        uint8_t H_LACTIVE : 1;
        uint8_t BDU       : 1;
        uint8_t BOOT      : 1;
      } ctrl_3;

      struct {
        uint8_t STOP_ON_FTH : 1;
        uint8_t reserved    : 1;
        uint8_t I2C_disable : 1;
        uint8_t DRDY_MASK   : 1;
        uint8_t FIFO_TEMP_EN  : 1;
        uint8_t INT2_on_INT1  : 1;
        uint8_t SLEEP_G       : 1;
        uint8_t XL_BW_SCAL_ODR  : 1;
      } ctrl_4;

      struct {
        uint8_t ST_XL   : 2;
        uint8_t ST_G    : 2;
        uint8_t reserved : 1;
        uint8_t ROUNDING : 3;
      } ctrl_5;

      struct {
        uint8_t reserved  : 4;
        uint8_t XL_HM_MODE  : 1;
        uint8_t LVL2_EN     : 1;
        uint8_t LVLen       : 1;
        uint8_t TRIG_EN     : 1;
      } ctrl_6;

      struct {
        uint8_t reserved  : 2;
        uint8_t ROUNDING_STATUS   : 1;
        uint8_t HP_G_RST          : 1;
        uint8_t HPCF_G            : 2;
        uint8_t HP_G_EN           : 1;
        uint8_t G_HM_MODE         : 1;
      } ctrl_7;

      struct {
        uint8_t LOW_PASS_ON_6D  : 1;
        uint8_t reserved0       : 1;
        uint8_t HP_SLOPE_XL_EN  : 1;
        uint8_t reserved1       : 2;
        uint8_t HPCF_XL         : 2;
        uint8_t LPF2_XL_EN      : 1;
      } ctrl_8;

      struct {
        uint8_t reserved0 : 3;
        uint8_t Xen_XL    : 1;
        uint8_t Yen_XL    : 1;
        uint8_t Zen_XL    : 1;
        uint8_t reserved1 : 2;
      } ctrl_9;

      struct {
        uint8_t SIGN_MOTION_EN  : 1;
        uint8_t PEDO_RST_STEP   : 1;
        uint8_t FUNC_EN         : 1;
        uint8_t Xen_G           : 1;
        uint8_t Yen_G           : 1;
        uint8_t Zen_G           : 1;
        uint8_t reserved        : 2;
      } ctrl_10;
    } registers;
    uint8_t data[10];

    CtrlRegisters();
  };

  inline uint8_t MakeCommandByte(RegisterMap reg_addr, bool do_read) {
    return ((uint8_t) reg_addr) | (do_read & 0x01);
  };

  uint8_t ReadAddress(RegisterMap reg_addr);

  /* Variables */
  static const uint8_t kIDCode = 0x69;
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