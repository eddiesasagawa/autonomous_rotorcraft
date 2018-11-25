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

GpioBase::~GpioBase() {
    if (0 == --ref_count_) {
        logger_->info("GPIO -- terminating pigpio.");
        gpioTerminate();
    }
}

}}} //arc::arc::bsp