#include <stdio.h>

#include <pigpio.h>
#include "gpio_base.hh"

namespace arc { namespace arc { namespace bsp {

const std::string GpioBase::kLogName = "gpio";
int16_t GpioBase::ref_count_ = 0;

GpioBase::GpioBase()
: logger_(common::Log::RetrieveLogger(kLogName)) {
    if (0 == ref_count_++) {
        logger_->info("GPIO -- initializing pigpio.");
        gpioInitialise();
    }
}

GpioBase::GpioBase(std::string log_name)
: logger_(common::Log::RetrieveLogger(log_name)) {
    if (0 == ref_count_++) {
        logger_->info("GPIO -- initializing pigpio.");
        gpioInitialise();
    }
}

GpioBase::~GpioBase() {
    if (0 == --ref_count_) {
        logger_->info("GPIO -- terminating pigpio.");
        gpioTerminate();
    }
}

common::ArcErrorCodes GpioBase::MapReturnCode(
    int gpio_ret_val   /**< [in] return code from gpio library */
) {
    common::ArcErrorCodes err_code = common::kArcErrorUnknownCode;
    /* pigpio error codes are negative */
    if (gpio_ret_val < 0) {
        switch (gpio_ret_val) {
            case PI_BAD_GPIO:
                err_code = common::kArcErrorGpioBadPin;
                break;
            case PI_BAD_MODE:
                err_code = common::kArcErrorGpioBadMode;
                break;
            case PI_BAD_DUTYCYCLE:
                err_code = common::kArcErrorGpioBadPwmCommand;
                break;
            case PI_BAD_USER_GPIO:
                err_code = common::kArcErrorGpioBadPinInput;
                break;
            case PI_BAD_DUTYRANGE:
                err_code = common::kArcErrorGpioBadPwmRange;
                break;
            case PI_BAD_LEVEL:
                err_code = common::kArcErrorGpioBadLevel;
                break;
        }
    } else {
        err_code = common::kArcErrorNone;
    }
    return err_code;
}


}}} //arc::arc::bsp