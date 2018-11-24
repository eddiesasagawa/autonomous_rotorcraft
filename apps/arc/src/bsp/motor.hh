#ifndef ARC_MOTOR_HH
#define ARC_MOTOR_HH

#include "gpio_base.hh"

namespace arc { namespace arc { namespace bsp {

class Motor : public GpioBase {
    public:
        Motor(
            uint16_t pwm_pin /**< [in] pin ID for PWM */
        );
        ~Motor();

        bool SetPWM(
            uint16_t duty_cycle /**< [in] PWM duty cycle,  */
        );

    protected:
        uint16_t pwm_pin_id_;
};

class UnidirectionalMotor : public Motor {
    public:
        UnidirectionalMotor(
            uint16_t pwm_pin /**< [in] pin ID for PWM */
        );
        ~UnidirectionalMotor();

};

class BidirectionalMotor : public Motor {
    public:
        enum Direction {
            kMotorDirPositive = 0,
            kMotorDirNegative = 1
        };

        BidirectionalMotor(
            uint16_t pwm_pin,   /**< [in] pin ID for PWM output */
            uint16_t dir_pin    /**< [in] pin ID for direction output */
        );
        ~BidirectionalMotor();

        bool SetDirection(Direction dir);
        
    private:
        uint16_t dir_pin_id_;
};

}}} //arc::arc::bsp

#endif //ARC_MOTOR_HH