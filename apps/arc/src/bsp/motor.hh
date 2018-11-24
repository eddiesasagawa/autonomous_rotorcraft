#ifndef ARC_MOTOR_HH
#define ARC_MOTOR_HH

#include "gpio_base.hh"

namespace arc { namespace arc { namespace bsp {

class Motor : public GpioBase {
    public:
        Motor(uint16_t gpio);
        ~Motor();

        bool SetPWM(uint16_t duty_cycle) { return gpioPWM(gpio_num_, duty_cycle) == 0; }

    protected:
        uint16_t gpio_num_;
};

class UnidirectionalMotor : public Motor {
    public:
        UnidirectionalMotor();
        ~UnidirectionalMotor();

};

class BidirectionalMotor : public Motor {
    public:
        BidirectionalMotor();
        ~BidirectionalMotor();
};

}}} //arc::arc::bsp

#endif //ARC_MOTOR_HH