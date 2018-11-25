#include "motor.hh"

#include <pigpio.h>
#include <assert.h>

namespace arc { namespace arc { namespace bsp {

Motor::Motor(
    uint16_t pwm_pin /**< [in] pin ID for PWM */
) : GpioBase(),
    pwm_pin_id_(pwm_pin) 
{
    /* Configure the pin for output */
    /* Make sure there were no issues with using the pin */
    assert(common::kArcErrorNone == common::Log::PrintOnError(
        logger_, __FILE__, __LINE__, 
        MapReturnCode(gpioSetMode(pwm_pin_id_, PI_OUTPUT))
    ));

    /* Set frequency to 8kHz */
    int pwm_freq_hz = gpioSetPWMfrequency(pwm_pin_id_, 8000);
    common::Log::PrintOnError(
        logger_, __FILE__, __LINE__, 
        MapReturnCode(pwm_freq_hz)
    );

    /* Set duty cycle max range to 1000 */
    int pwm_duty_range = gpioSetPWMrange(pwm_pin_id_, 1000);
    common::Log::PrintOnError(
        logger_, __FILE__, __LINE__,
        MapReturnCode(pwm_duty_range)
    );

    logger_->info("MOTOR -- motor pwm pin #%d to %d hz and range %d.", pwm_pin_id_, pwm_freq_hz, pwm_duty_range);
}

Motor::~Motor() {
    /* ... */
}

common::ArcErrorCodes Motor::SetPWM(
    uint16_t duty_cycle /**< [in] PWM duty cycle,  */
) {
    common::Log::PrintOnError(
        logger_, __FILE__, __LINE__,
        MapReturnCode(gpioPWM(pwm_pin_id_, duty_cycle))
    );
}

/************************************************************************************/
UnidirectionalMotor::UnidirectionalMotor(uint16_t pwm_pin) : Motor(pwm_pin) {}

UnidirectionalMotor::~UnidirectionalMotor() {}

/************************************************************************************/
BidirectionalMotor::BidirectionalMotor(uint16_t pwm_pin, uint16_t dir_pin)
: Motor(pwm_pin), dir_pin_id_(dir_pin) {
    /* configure the direction pin */
    assert(common::kArcErrorNone == common::Log::PrintOnError(
        logger_, __FILE__, __LINE__, 
        MapReturnCode(gpioSetMode(dir_pin_id_, PI_OUTPUT))
    ));

    /* Make sure pin is off */
    common::Log::PrintOnError(
        logger_, __FILE__, __LINE__,
        MapReturnCode(gpioWrite(dir_pin_id_, kMotorDirNegative))
    );
}

BidirectionalMotor::~BidirectionalMotor() {}

common::ArcErrorCodes BidirectionalMotor::SetDirection(BidirectionalMotor::Direction dir) {
    common::Log::PrintOnError(
        logger_, __FILE__, __LINE__,
        MapReturnCode(gpioWrite(dir_pin_id_, dir))
    );
}


}}} //arc::arc::bsp