#ifndef COMM_LINK_INTERFACE_HH
#define COMM_LINK_INTERFACE_HH

#include "gpio_base.hh"
#include "arc_errors.hh"

namespace arc {
  namespace arc {
    namespace bsp {

/**
 * General / abstract interface class for communication buses (SPI, UART, I2C, etc.)
 */
class CommLinkInterface {
 public:
  // CommLinkInterface();

  virtual common::ArcErrorCodes Receive(
    char* const rx_data,
    const uint16_t num_bytes
  ) = 0;
  virtual common::ArcErrorCodes Transmit(
    char* const tx_data,
    const uint16_t num_bytes
  ) = 0;
  virtual common::ArcErrorCodes Communicate(
    char* const tx_data,
    char* const rx_data,
    const uint16_t num_bytes
  ) = 0;
};

/**
 * Unused. Left as placeholder
 */
class I2CInterface : public CommLinkInterface, public GpioBase {

};

/**
 * SPI Interface wrapper
 */
class SPIInterface : public CommLinkInterface, public GpioBase {
 public:
  struct SpiBusSettings {
    uint8_t channel;
    uint32_t baud;
    union {
      struct {
        uint32_t  mode                    : 2;
        uint32_t  ce0_set_active_high     : 1;
        uint32_t  ce1_set_active_high     : 1;
        uint32_t  ce2_set_active_high     : 1;
        uint32_t  ce0_dont_reserve        : 1;
        uint32_t  ce1_dont_reserve        : 1;
        uint32_t  ce2_dont_reserve        : 1;
        uint32_t  is_aux_spi_bus          : 1;
        uint32_t  is_3wire_mode           : 1;
        uint32_t  num_bytes_write_3wire   : 4;
        uint32_t  aux_spi_tx_lsbit_first  : 1;
        uint32_t  aux_spi_rx_lsbit_first  : 1;
        uint32_t  aux_spi_word_num_bits   : 6;
        uint32_t  spare   : 10;
      } flags;
      uint32_t flag_set;
    };
  };

  SPIInterface(
    std::string name,
    SpiBusSettings settings
  );
  ~SPIInterface();

  common::ArcErrorCodes Receive(
    char* const rx_data,
    const uint16_t num_bytes
  ) override;
  common::ArcErrorCodes Transmit(
    char* const tx_data,
    const uint16_t num_bytes
  ) override;
  common::ArcErrorCodes Communicate(
    char* const tx_data,
    char* const rx_data,
    const uint16_t num_bytes
  ) override;

 private:
  SpiBusSettings settings_;
  int spi_handle_;
};

    } // arc::arc::bsp
  } // arc::arc
}

#endif //COMM_LINK_INTERFACE_HH