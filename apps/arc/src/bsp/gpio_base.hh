#ifndef ARC_GPIO_BASE_HH
#define ARC_GPIO_BASE_HH

#include <pigpio.h>

namespace arc { namespace arc { namespace bsp {

class GpioBase {
    public:
        GpioBase();
        ~GpioBase();

    protected:
        static int16_t ref_count_;
};

}}} 

#endif //ARC_GPIO_BASE_HH