#include <stdio.h>

#include <pigpio.h>
#include "gpio_base.hh"

namespace arc { namespace arc { namespace bsp {

int16_t GpioBase::ref_count_ = 0;

GpioBase::GpioBase() {
    if (0 == ref_count_++) {
        printf("GPIO -- initializing pigpio.\n");
        gpioInitialise();
    }
}

GpioBase::~GpioBase() {
    if (0 == --ref_count_) {
        printf("GPIO -- terminating pigpio.\n");
        gpioTerminate();
    }
}

}}} //arc::arc::bsp