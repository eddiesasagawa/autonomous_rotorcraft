#ifndef SENSOR_ST_LSM6DS33_DEFS_HH
#define SENSOR_ST_LSM6DS33_DEFS_HH

#include <stdint.h>

namespace arc {
  namespace arc {
    namespace bsp {
      namespace st_lsm6ds33_defs {

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
  kLSM6DS33Addr_OUTZ_H_XL = 0x2D,

  kLSM6DS33Addr_TIMESTAMP0 = 0x40,
  kLSM6DS33Addr_TIMESTAMP1 = 0x41,
  kLSM6DS33Addr_TIMESTAMP2 = 0x42,

  kLSM6DS33Addr_TAP_CFG = 0x58
};

enum AccelerometerFullScale {
  kLSM6DS33_XLFS_2G  = 0,
  kLSM6DS33_XLFS_16G = 1,
  kLSM6DS33_XLFS_4G  = 2,
  kLSM6DS33_XLFS_8G  = 3
};

enum AccelerometerDataRate {
  kLSM6DS33_XLODR_OFF    = 0,
  kLSM6DS33_XLODR_13HZ   = 1,
  kLSM6DS33_XLODR_26HZ   = 2,
  kLSM6DS33_XLODR_52HZ   = 3,
  kLSM6DS33_XLODR_104HZ  = 4,
  kLSM6DS33_XLODR_208HZ  = 5,
  kLSM6DS33_XLODR_416HZ  = 6,
  kLSM6DS33_XLODR_833HZ  = 7,
  kLSM6DS33_XLODR_1660HZ = 8,
  kLSM6DS33_XLODR_3330HZ = 9,
  kLSM6DS33_XLODR_6660HZ = 10
};

enum GyroscopeFullScale {
  kLSM6DS33_GFS_245DPS   = 0,
  kLSM6DS33_GFS_500DPS   = 1,
  kLSM6DS33_GFS_1000DPS  = 2,
  kLSM6DS33_GFS_2000DPS  = 3
};

enum GyroscopeDataRate {
  kLSM6DS33_GODR_OFF    = 0,
  kLSM6DS33_GODR_13HZ   = 1,
  kLSM6DS33_GODR_26HZ   = 2,
  kLSM6DS33_GODR_52HZ   = 3,
  kLSM6DS33_GODR_104HZ  = 4,
  kLSM6DS33_GODR_208HZ  = 5,
  kLSM6DS33_GODR_416HZ  = 6,
  kLSM6DS33_GODR_833HZ  = 7,
  kLSM6DS33_GODR_1660HZ = 8
};

enum TimestampResolution {
  kLSM6DS33_TIME_LSB_6400us = 0,
  kLSM6DS33_TIME_LSB_25us = 1
};

/**** Register Definitions  ****/
union TapConfigRegister {
  struct {
    uint8_t LIR       : 1;
    uint8_t TAP_Z_EN  : 1;
    uint8_t TAP_Y_EN  : 1;
    uint8_t TAP_X_EN  : 1;
    uint8_t SLOPE_FDS : 1;
    uint8_t TILT_EN   : 1;
    uint8_t PEDO_EN   : 1;
    uint8_t TIMER_EN  : 1;
  } TAP_CFG;
  uint8_t byte;
};

union DurationSettingsRegister {
  struct {
    uint8_t SLEEP_DUR : 4;
    uint8_t TIMER_HR  : 1;
    uint8_t WAKE_DUR  : 2;
    uint8_t FF_DUR5   : 1;
  } WAKE_UP_DUR;
  uint8_t byte;
};

union CtrlRegisters {
  struct {
    uint8_t cmd_word_placeholder;

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
  uint8_t data[10+1];

  CtrlRegisters();
};

union DataRegisters {
  struct {
    uint8_t cmd_word_placeholder;

    uint8_t OUT_TEMP_L;
    uint8_t OUT_TEMP_H;
    uint8_t OUTX_L_G;
    uint8_t OUTX_H_G;
    uint8_t OUTY_L_G;
    uint8_t OUTY_H_G;
    uint8_t OUTZ_L_G;
    uint8_t OUTZ_H_G;
    uint8_t OUTX_L_XL;
    uint8_t OUTX_H_XL;
    uint8_t OUTY_L_XL;
    uint8_t OUTY_H_XL;
    uint8_t OUTZ_L_XL;
    uint8_t OUTZ_H_XL;
  } registers;
  uint8_t data[15];
};

union TimeRegisters {
  struct {
    uint8_t cmd_word_placeholder;
    uint8_t TIMESTAMP0_REG;
    uint8_t TIMESTAMP1_REG;
    uint8_t TIMESTAMP2_REG;
  } registers;
  uint8_t data[4];
};

      } // st_lsm6ds33_defs
    }
  }
}

#endif