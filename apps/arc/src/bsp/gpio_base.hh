#ifndef ARC_GPIO_BASE_HH
#define ARC_GPIO_BASE_HH

#include "arc_errors.hh"

namespace arc { namespace arc { namespace bsp {

class GpioBase {
 public:
  GpioBase();
  GpioBase(std::string log_name);
  ~GpioBase();

 protected:
  /**
   * This function converts gpio library return codes to ARC error codes
   */
  common::ArcErrorCodes MapReturnCode(
    int gpio_ret_code   /**< [in] return code from gpio library */
  );

  common::shared_log_ptr_t logger_;
  static const std::string kLogName;

  static int16_t ref_count_;
};

}}}

#endif //ARC_GPIO_BASE_HH