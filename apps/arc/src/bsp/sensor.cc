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
  readings(ImuMeasurements()),
  time_us_lsb_(6400.0),
  raw_time_(TimeRegisters()),
  raw_data_(DataRegisters()),
  ctrl_settings_(CtrlRegisters())
{
  logger_->set_level(spdlog::level::debug);

  /* Query the sensor for version */
  logger_->info("Query sensor version: 0x{:02x}", ReadRegister(kLSM6DS33Addr_WHO_AM_I));

  /* Read back the default settings and set new ones */
  ctrl_settings_ = GrabCtrlSettings();
  ctrl_settings_.registers.ctrl_1.FS_XL = kLSM6DS33_XLFS_4G;
  ctrl_settings_.registers.ctrl_1.ODR_XL = kLSM6DS33_XLODR_104HZ;
  ctrl_settings_.registers.ctrl_2.FS_G = kLSM6DS33_GFS_245DPS;
  ctrl_settings_.registers.ctrl_2.ODR_G = kLSM6DS33_GODR_104HZ;
  // ctrl_settings_.registers.ctrl_3.BDU = 0; // enable Block Data update (don't update until both MSB and LSB are ready)
  LoadCtrlSettings(ctrl_settings_);

  /* Enable the timestamp */
  TapConfigRegister tap_cfg = {0};
  tap_cfg.TAP_CFG.TIMER_EN = 1;
  WriteRegister(kLSM6DS33Addr_TAP_CFG, tap_cfg.byte);
  /* Reset the timer */
  WriteRegister(kLSM6DS33Addr_TIMESTAMP2, 0xAA);
}

ST_6DOFImu_LSM6DS33::~ST_6DOFImu_LSM6DS33() {}

void ST_6DOFImu_LSM6DS33::Query() {
  /* Read IMU data */
  GrabData();

  logger_->info("Time: {:f} s", readings.t_valid);
  logger_->info("Temperature: {:02.3f} degrees C", readings.temperature);
  logger_->info("Accelerometer: < {:03.6f}, {:03.6f}, {:03.6f} > m/s^2", readings.f_im_m(0), readings.f_im_m(1), readings.f_im_m(2));
  logger_->info("Gyroscope: < {:03.6f}, {:03.6f}, {:03.6f} > rad/s", readings.omega_im_m(0), readings.omega_im_m(1), readings.omega_im_m(2));
}

uint8_t ST_6DOFImu_LSM6DS33::ReadRegister(ST_6DOFImu_LSM6DS33::RegisterMap reg_addr) {
  uint8_t read_buf[2] = {0xFF, 0xFF};
  uint8_t tx_buf[2] = {MakeCommandByte(reg_addr, true), 0};
  Communicate((char *)tx_buf, (char *)read_buf, 2);
  // logger_->info("ReadRegister: cmd-{:02x},{:02x}  rx-{:02x},{:02x}", tx_buf[0], tx_buf[1], read_buf[0], read_buf[1]);
  return read_buf[1];
}

void ST_6DOFImu_LSM6DS33::WriteRegister(RegisterMap reg_addr, uint8_t new_val){
  uint8_t tx_buf[2] = {MakeCommandByte(reg_addr, false), new_val};
  Transmit((char *)tx_buf, 2);
}

CtrlRegisters ST_6DOFImu_LSM6DS33::GrabCtrlSettings() {
  CtrlRegisters current_settings = CtrlRegisters();
  uint8_t tx_buf[sizeof(CtrlRegisters)] = {MakeCommandByte(kLSM6DS33Addr_CTRL1_XL, true), 0};
  Communicate((char *)tx_buf, (char *)(current_settings.data), sizeof(CtrlRegisters));
  return current_settings;
}

void ST_6DOFImu_LSM6DS33::LoadCtrlSettings(CtrlRegisters new_settings) {
  new_settings.registers.cmd_word_placeholder = MakeCommandByte(kLSM6DS33Addr_CTRL1_XL, false);
  Transmit((char *)(new_settings.data), sizeof(CtrlRegisters));
}

void ST_6DOFImu_LSM6DS33::GrabData() {
  float imu_scale = 0;
  uint8_t tx_buf[sizeof(DataRegisters)] = {MakeCommandByte(kLSM6DS33Addr_OUT_TEMP_L, true), 0};
  Communicate((char *)tx_buf, (char *)(raw_data_.data), sizeof(DataRegisters));
  // PrintByteArray(raw_data_.data, sizeof(DataRegisters), 1, kLSM6DS33Addr_OUT_TEMP_L-1);

  tx_buf[0] = MakeCommandByte(kLSM6DS33Addr_TIMESTAMP0, true);
  Communicate((char *)tx_buf, (char *)(raw_time_.data), sizeof(TimeRegisters));
  // PrintByteArray(raw_time_.data, sizeof(TimeRegisters), 1, kLSM6DS33Addr_TIMESTAMP0-1);

  /* Time */
  readings.t_valid = time_us_lsb_ * (double)((((uint32_t)raw_time_.registers.TIMESTAMP2_REG) << 16)
                                    | (((uint32_t)raw_time_.registers.TIMESTAMP1_REG) << 8)
                                    | (((uint32_t)raw_time_.registers.TIMESTAMP0_REG)));
  readings.t_valid /= 1000000; // convert to seconds

  /* Temperature: signed 2's complement */
  readings.temperature = 25.0 + DecodeInt16<float>(
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
  readings.omega_im_m(0) = (3.14/180.0/1000.0)* DecodeInt16<float>(
    (((int16_t)raw_data_.registers.OUTX_H_G) << 8) | ((int16_t)raw_data_.registers.OUTX_L_G),
    imu_scale);
  readings.omega_im_m(1) = (3.14/180.0/1000.0)* DecodeInt16<float>(
    (((int16_t)raw_data_.registers.OUTY_H_G) << 8) | ((int16_t)raw_data_.registers.OUTY_L_G),
    imu_scale);
  readings.omega_im_m(2) = (3.14/180.0/1000.0)* DecodeInt16<float>(
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

  readings.f_im_m(0) = (9.81/1000.0) * DecodeInt16<float>(
    (((int16_t)raw_data_.registers.OUTX_H_XL) << 8) | ((int16_t)raw_data_.registers.OUTX_L_XL),
    imu_scale);
  readings.f_im_m(1) = (9.81/1000.0) * DecodeInt16<float>(
    (((int16_t)raw_data_.registers.OUTY_H_XL) << 8) | ((int16_t)raw_data_.registers.OUTY_L_XL),
    imu_scale);
  readings.f_im_m(2) = (9.81/1000.0) * DecodeInt16<float>(
    (((int16_t)raw_data_.registers.OUTZ_H_XL) << 8) | ((int16_t)raw_data_.registers.OUTZ_L_XL),
    imu_scale);
}

void ST_6DOFImu_LSM6DS33::PrintByteArray(const uint8_t* const data, uint16_t num_bytes, uint16_t offset, uint8_t start_addr) {
  logger_->debug("Byte array dump: ");
  for (uint16_t i = offset; i < num_bytes; i++) {
    logger_->debug("  {:02X}h = 0x{:02x}", start_addr+i, data[i]);
  }
}

    } //arc::arc::bsp
  }
}
