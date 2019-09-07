#include "comm_link_interface.hh"
#include <pigpio.h>
#include <assert.h>

namespace arc {
  namespace arc {
    namespace bsp {

SPIInterface::SPIInterface(
  std::string name,
  SpiBusSettings settings
) : GpioBase(name),
    settings_(settings),
    spi_handle_( spiOpen(settings.channel, settings.baud, settings.flag_set) )
{
  /* Verify that the SPI handle opened correctly */
  assert(common::kArcErrorNone == common::Log::PrintOnError(
      logger_, __FILE__, __LINE__,
      MapReturnCode(spi_handle_)
  ));

  logger_->info("starting on {} SPI bus", settings.flags.is_aux_spi_bus ? "AUX" : "MAIN");
}

SPIInterface::~SPIInterface() {
  common::Log::PrintOnError(
      logger_, __FILE__, __LINE__,
      MapReturnCode(spiClose(spi_handle_))
  );
}

common::ArcErrorCodes SPIInterface::Receive(
  char* const rx_data,
  const uint16_t num_bytes
) {
  common::ArcErrorCodes err_code = MapReturnCode(
    spiRead(spi_handle_, rx_data, num_bytes)
  );
  common::Log::PrintOnError(logger_, __FILE__, __LINE__, err_code);
  return err_code;
}

common::ArcErrorCodes SPIInterface::Transmit(
  char* const tx_data,
  const uint16_t num_bytes
) {
  common::ArcErrorCodes err_code = MapReturnCode(
    spiWrite(spi_handle_, tx_data, num_bytes)
  );
  common::Log::PrintOnError(logger_, __FILE__, __LINE__, err_code);
  return err_code;
}

common::ArcErrorCodes SPIInterface::Communicate(
  char* const tx_data,
  char* const rx_data,
  const uint16_t num_bytes
) {
  int ret = spiXfer(spi_handle_, tx_data, rx_data, num_bytes);
  common::ArcErrorCodes err_code = MapReturnCode(ret);
  common::Log::PrintOnError(logger_, __FILE__, __LINE__, err_code);
  return err_code;
}

    } // arc::arc::bsp
  }
}