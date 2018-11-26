#include "motor.hh"

#include <pigpio.h>
#include <assert.h>

namespace arc { namespace arc { namespace bsp {

Motor::Motor(
    uint16_t pwm_pin /**< [in] pin ID for PWM */
) : GpioBase("motor"),
    pwm_pin_id_(pwm_pin)
{
    /* Configure the pin for output */
    /* Make sure there were no issues with using the pin */
    assert(common::kArcErrorNone == common::Log::PrintOnError(
        logger_, __FILE__, __LINE__,
        MapReturnCode(gpioSetMode(pwm_pin_id_, PI_OUTPUT))
    ));

    /* Set frequency */
    int pwm_freq_hz = gpioSetPWMfrequency(pwm_pin_id_, 2000);
    common::Log::PrintOnError(
        logger_, __FILE__, __LINE__,
        MapReturnCode(pwm_freq_hz)
    );

    /* Set duty cycle max range to 1000 */
    int pwm_real_range = gpioSetPWMrange(pwm_pin_id_, 100);
    int pwm_range = gpioGetPWMrange(pwm_pin_id_);
    // int pwm_real_range = gpioGetPWMrange(pwm_pin_id_);
    common::Log::PrintOnError(
        logger_, __FILE__, __LINE__,
        MapReturnCode(pwm_real_range)
    );

    logger_->info("motor pwm pin #{:d}: {:d} hz, range {:d} (real {:d}).", pwm_pin_id_, pwm_freq_hz, pwm_range, pwm_real_range);
}

Motor::~Motor() {
    /* ... */
}

common::ArcErrorCodes Motor::SetPWM(
    uint16_t duty_cycle /**< [in] PWM duty cycle,  */
) {
    common::ArcErrorCodes err_code = MapReturnCode(gpioPWM(pwm_pin_id_, duty_cycle));
    common::Log::PrintOnError(logger_, __FILE__, __LINE__, err_code);
    return err_code;
}

/************************************************************************************/
UnidirectionalMotor::UnidirectionalMotor(uint16_t pwm_pin) : Motor(pwm_pin) {
    logger_->info("constructed uni-directional motor: PWM#{}", pwm_pin);
}

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

    logger_->info("constructed bi-directional motor: PWM#{}, DIR#{}", pwm_pin, dir_pin);
}

BidirectionalMotor::~BidirectionalMotor() {}

common::ArcErrorCodes BidirectionalMotor::SetDirection(BidirectionalMotor::Direction dir) {
    common::ArcErrorCodes err_code = MapReturnCode(gpioWrite(dir_pin_id_, dir));
    common::Log::PrintOnError(logger_, __FILE__, __LINE__, err_code);
    return err_code;
}


}}} //arc::arc::bsp