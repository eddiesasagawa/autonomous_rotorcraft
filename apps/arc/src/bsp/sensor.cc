#include "sensor.hh"

namespace arc {
  namespace arc {
    namespace bsp {

/******************************************************/
using namespace st_lsm6ds33_defs;

CtrlRegisters::CtrlRegisters()
: data {
    0x00,
    0x00, /* CTRL 1 default */
    0x00, /* CTRL 2 default */
    0x04, /* CTRL 3 default */
    0x00, /* CTRL 4 default */
    0x00, /* CTRL 5 default */
    0x00, /* CTRL 6 default */
    0x00, /* CTRL 7 default */
    0x00, /* CTRL 8 default */
    0x38, /* CTRL 9 default */
    0x38  /* CTRL 10 default */
  }
{ }

/******************************************************/
ST_6DOFImu_LSM6DS33::ST_6DOFImu_LSM6DS33()
: SPIInterface(
    "LSM6DS33",
    { /* I guess C++ does not have designated initiallizers until C++20 */
      0,          /* channel */
      10000000,   /* baud */
      {
        /* According to user manual:
            - CLK stops high when CS is high (no transmission)
                  --> CPOL = 1
            - Toggle on falling edge (leading edge), capture at rising edge
                  --> CPHA = 1
        */
        3,  /* SPI mode */
        false,  /* ce0 set active high */
        false,  /* ce1 set active high */
        false,  /* ce2 set active high */
        false,  /* ce0 don't reserve */
        true,   /* ce1 don't reserve */
        true,   /* ce2 don't reserve */
        false,  /* is aux spi bus */
        false   /* is 3 wire mode */
        /* leave rest at default values */
      }
    }
  ),
  time_us_lsb_(6400.0),
  raw_data_(DataRegisters()),
  ctrl_settings_(CtrlRegisters())
{
  logger_->set_level(spdlog::level::debug);

  /* Query the sensor for version */
  logger_->info("Query sensor version: 0x{:02x}", ReadAddress(kLSM6DS33Addr_WHO_AM_I));

  /* Read back the default settings and set new ones */
  ctrl_settings_ = GrabCtrlSettings();
  ctrl_settings_.registers.ctrl_1.FS_XL = kLSM6DS33_XLFS_4G;
  ctrl_settings_.registers.ctrl_1.ODR_XL = kLSM6DS33_XLODR_104HZ;
  ctrl_settings_.registers.ctrl_2.FS_G = kLSM6DS33_GFS_245DPS;
  ctrl_settings_.registers.ctrl_2.ODR_G = kLSM6DS33_GODR_104HZ;
  // ctrl_settings_.registers.ctrl_3.BDU = 0; // enable Block Data update (don't update until both MSB and LSB are ready)
  LoadCtrlSettings(ctrl_settings_);
}

ST_6DOFImu_LSM6DS33::~ST_6DOFImu_LSM6DS33() {}

void ST_6DOFImu_LSM6DS33::Query() {
  logger_->info("Query sensor version: 0x{:02x}", ReadAddress(kLSM6DS33Addr_WHO_AM_I));

  /* Read IMU data */
  GrabData();

  logger_->info("Temperature: {:02.3f} degC", temperature);
  logger_->info("Accelerometer: < {:03.6f}, {:03.6f}, {:03.6f} > mm/s^2", f_imx_m, f_imy_m, f_imz_m);
  logger_->info("Gyroscope: < {:03.6f}, {:03.6f}, {:03.6f} > mdeg/s", omega_imx_m, omega_imy_m, omega_imz_m);
}

uint8_t ST_6DOFImu_LSM6DS33::ReadAddress(ST_6DOFImu_LSM6DS33::RegisterMap reg_addr) {
  uint8_t read_buf[2] = {0xFF, 0xFF};
  uint8_t tx_buf[2] = {MakeCommandByte(reg_addr, true), 0};
  Communicate((char *)tx_buf, (char *)read_buf, 2);
  // logger_->info("ReadAddress: cmd-{:02x},{:02x}  rx-{:02x},{:02x}", tx_buf[0], tx_buf[1], read_buf[0], read_buf[1]);
  return read_buf[1];
}

CtrlRegisters ST_6DOFImu_LSM6DS33::GrabCtrlSettings() {
  CtrlRegisters current_settings = CtrlRegisters();
  uint8_t tx_buf[sizeof(CtrlRegisters)] = {MakeCommandByte(kLSM6DS33Addr_CTRL1_XL, true), 0};
  Communicate((char *)tx_buf, (char *)(current_settings.data), sizeof(CtrlRegisters));

  // logger_->info("IMU CTRL registers:");
  // for (int i = 0; i < 10; i++) {
  //   logger_->info("  CTRL{:02d} = 0x{:02x}", i+1, ctrl_reg_ptr->data[i]);
  // }

  return current_settings;
}

void ST_6DOFImu_LSM6DS33::LoadCtrlSettings(CtrlRegisters new_settings) {
  new_settings.registers.cmd_word_placeholder = MakeCommandByte(kLSM6DS33Addr_CTRL1_XL, false);
  Transmit((char *)(new_settings.data), sizeof(CtrlRegisters));
}

void ST_6DOFImu_LSM6DS33::GrabData() {
  float imu_scale = 0;
  uint8_t tx_buf[sizeof(DataRegisters)] = {MakeCommandByte(kLSM6DS33Addr_STATUS_REG, true), 0};
  Communicate((char *)tx_buf, (char *)(raw_data_.data), sizeof(DataRegisters)+1);

  // logger_->info("IMU data registers:");
  // for (int i = 1; i < 16; i++) {
  //   logger_->info("  {:02d} = 0x{:02x}", i, raw_data_.data[i]);
  // }

  /* Temperature: signed 2's complement */
  temperature = 25.0 + ConvertInt16ToFloat(
    (((int16_t)raw_data_.registers.OUT_TEMP_H) << 8) | ((int16_t)raw_data_.registers.OUT_TEMP_L),
    (1.0 / 16.0));
  /* Gyroscope: signed 2's complement */
  if (ctrl_settings_.registers.ctrl_2.FS_125) {
    imu_scale = 4.375;
  } else {
    switch (ctrl_settings_.registers.ctrl_2.FS_G) {
      case kLSM6DS33_GFS_245DPS:
        imu_scale = 8.75;
        break;
      case kLSM6DS33_GFS_500DPS:
        imu_scale = 17.5;
        break;
      case kLSM6DS33_GFS_1000DPS:
        imu_scale = 35.0;
        break;
      case kLSM6DS33_GFS_2000DPS:
        imu_scale = 70.0;
        break;
    }
  }
  logger_->debug("gyro lsb = {:f}", imu_scale);
  omega_imx_m = ConvertInt16ToFloat(
    (((int16_t)raw_data_.registers.OUTX_H_G) << 8) | ((int16_t)raw_data_.registers.OUTX_L_G),
    imu_scale);
  omega_imy_m = ConvertInt16ToFloat(
    (((int16_t)raw_data_.registers.OUTY_H_G) << 8) | ((int16_t)raw_data_.registers.OUTY_L_G),
    imu_scale);
  omega_imz_m = ConvertInt16ToFloat(
    (((int16_t)raw_data_.registers.OUTZ_H_G) << 8) | ((int16_t)raw_data_.registers.OUTZ_L_G),
    imu_scale);
  /* Accelerometer: signed 2's complement */
  switch (ctrl_settings_.registers.ctrl_1.FS_XL) { // mg/LSB
    case kLSM6DS33_XLFS_2G:
      imu_scale = 0.061;
      break;
    case kLSM6DS33_XLFS_4G:
      imu_scale = 0.122;
      break;
    case kLSM6DS33_XLFS_8G:
      imu_scale = 0.244;
      break;
    case kLSM6DS33_XLFS_16G:
      imu_scale = 0.488;
      break;
  }
  logger_->debug("accel lsb = {:f}", imu_scale);
  f_imx_m = 9.81 * ConvertInt16ToFloat(
    (((int16_t)raw_data_.registers.OUTX_H_XL) << 8) | ((int16_t)raw_data_.registers.OUTX_L_XL),
    imu_scale);
  f_imy_m = 9.81 * ConvertInt16ToFloat(
    (((int16_t)raw_data_.registers.OUTY_H_XL) << 8) | ((int16_t)raw_data_.registers.OUTY_L_XL),
    imu_scale);
  f_imz_m = 9.81 * ConvertInt16ToFloat(
    (((int16_t)raw_data_.registers.OUTZ_H_XL) << 8) | ((int16_t)raw_data_.registers.OUTZ_L_XL),
    imu_scale);
}


    } //arc::arc::bsp
  }
}
