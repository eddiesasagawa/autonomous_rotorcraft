#include "sensor.hh"

namespace arc {
  namespace arc {
    namespace bsp {

/******************************************************/
ST_6DOFImu_LSM6DS33::CtrlRegisters::CtrlRegisters()
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
  )
{
  /* Query the sensor for version */
  logger_->info("Query sensor version: {:02x}", ReadAddress(kLSM6DS33Addr_WHO_AM_I));

  // /* Read back the default settings */
  // uint8_t read_buf[sizeof(CtrlRegisters)+1] = {0};
  // uint8_t tx_buf[sizeof(CtrlRegisters)+1] = {MakeCommandByte(kLSM6DS33Addr_CTRL1_XL, true), 0x00};
  // Communicate((char *)tx_buf, (char *)read_buf, sizeof(CtrlRegisters)+1);

  // /* Update those settings */
  // CtrlRegisters* ctrl_reg_ptr = (CtrlRegisters*) &(read_buf[1]); // first byte is just throwaway?

}

ST_6DOFImu_LSM6DS33::~ST_6DOFImu_LSM6DS33() {}

void ST_6DOFImu_LSM6DS33::Query() {
  logger_->info("Query sensor version: {:02x}", ReadAddress(kLSM6DS33Addr_WHO_AM_I));
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
