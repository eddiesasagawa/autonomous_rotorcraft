#include "sensor.hh"

namespace arc {
  namespace arc {
    namespace bsp {

/******************************************************/
using namespace st_lsm6ds33_defs;

CtrlRegisters::CtrlRegisters()
: data {
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
  raw_data(DataRegisters())
{
  /* Query the sensor for version */
  logger_->info("Query sensor version: 0x{:02x}", ReadAddress(kLSM6DS33Addr_WHO_AM_I));

  /* Read back the default settings */
  uint8_t read_buf[sizeof(CtrlRegisters)+1] = {0};
  uint8_t tx_buf[sizeof(CtrlRegisters)+1] = {MakeCommandByte(kLSM6DS33Addr_CTRL1_XL, true), 0x00};
  Communicate((char *)tx_buf, (char *)read_buf, sizeof(CtrlRegisters)+1);
  CtrlRegisters* ctrl_reg_ptr = (CtrlRegisters*) &(read_buf[1]);
  // logger_->info("IMU CTRL registers:");
  // for (int i = 0; i < 10; i++) {
  //   logger_->info("  CTRL{:02d} = 0x{:02x}", i+1, ctrl_reg_ptr->data[i]);
  // }

  /* Set new options */
  ctrl_reg_ptr->registers.ctrl_1.ODR_XL = kLSM6DS33_XLODR_833HZ;
  ctrl_reg_ptr->registers.ctrl_2.ODR_G = kLSM6DS33_GODR_833HZ;
  ctrl_reg_ptr->registers.ctrl_3.BDU = 1; // enable Block Data update (don't update until both MSB and LSB are ready)
  memset(tx_buf, 0, sizeof(CtrlRegisters)+1);
  tx_buf[0] = MakeCommandByte(kLSM6DS33Addr_CTRL1_XL, false);
  memcpy(tx_buf+1, ctrl_reg_ptr, sizeof(CtrlRegisters));
  Transmit((char *)tx_buf, sizeof(CtrlRegisters)+1);

}

ST_6DOFImu_LSM6DS33::~ST_6DOFImu_LSM6DS33() {}

void ST_6DOFImu_LSM6DS33::Query() {
  logger_->info("Query sensor version: 0x{:02x}", ReadAddress(kLSM6DS33Addr_WHO_AM_I));

  /* Read IMU data */
  uint8_t read_buf[sizeof(DataRegisters)+1] = {0};
  uint8_t tx_buf[sizeof(DataRegisters)+1] = {MakeCommandByte(kLSM6DS33Addr_STATUS_REG, true), 0};
  Communicate((char *)tx_buf, (char *)read_buf, sizeof(DataRegisters)+1);
  memcpy(&raw_data, read_buf+1, sizeof(DataRegisters));

  logger_->info("Temperature: {:02.3f} degC", GetTemperature());
}

float ST_6DOFImu_LSM6DS33::GetTemperature() {
  /* Per spec sheet, LSM6DS33 temp range is -40 to +85 deg C */
  uint16_t val = (((uint16_t)raw_data.registers.OUT_TEMP_H) << 8) | ((uint16_t)raw_data.registers.OUT_TEMP_L);
  // 2's complement to decimal
  int16_t decval = (int16_t)(~(val)) + 1;
  if (decval & 0x8000) {
    decval = (decval & 0x7FFF);
  } else {
    decval = (decval | 0x8000);
  }
  float temp = ((float)decval * (85.0 + 40.0) / (65536.0));
  // logger_->info("GetTemp: {:d} -> {:d} -> {:f}", val, decval, temp);
  return temp;
}

uint8_t ST_6DOFImu_LSM6DS33::ReadAddress(ST_6DOFImu_LSM6DS33::RegisterMap reg_addr) {
  uint8_t read_buf[2] = {0xFF, 0xFF};
  uint8_t tx_buf[2] = {MakeCommandByte(reg_addr, true), 0};
  Communicate((char *)tx_buf, (char *)read_buf, 2);
  // logger_->info("ReadAddress: cmd-{:02x},{:02x}  rx-{:02x},{:02x}", tx_buf[0], tx_buf[1], read_buf[0], read_buf[1]);
  return read_buf[1];
}
    } //arc::arc::bsp
  }
}
