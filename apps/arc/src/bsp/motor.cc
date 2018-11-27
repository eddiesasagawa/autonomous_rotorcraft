#include "motor.hh"

#include <pigpio.h>
#include <assert.h>

namespace arc { namespace arc { namespace bsp {

Motor::Motor(
    std::string name,   /**< [in] name of motor */
    uint16_t pwm_pin /**< [in] pin ID for PWM */
) : GpioBase(name),
    pwm_pin_id_(pwm_pin),
    prev_cmd_pct_(0)
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

common::ArcErrorCodes Motor::SetMotorCmd(
    int16_t cmd_pct     /**< [in] PWM command percentage (0 to 100) */
) {
    if (cmd_pct > 100) {
        logger_->warn("motor command {} too high", cmd_pct);
        cmd_pct = 100;
    } else if (cmd_pct < 0) {
        logger_->warn("motor command {} less than 0", cmd_pct);
        cmd_pct = 0;
    }

    prev_cmd_pct_ = cmd_pct;
    return SetPWM(cmd_pct);
}

/************************************************************************************/
UnidirectionalMotor::UnidirectionalMotor(
    std::string name,   /**< [in] name of motor */
    uint16_t pwm_pin
) : Motor(name, pwm_pin)
{
    logger_->info("constructed uni-directional motor: PWM#{}", pwm_pin);
}

UnidirectionalMotor::~UnidirectionalMotor() {}

/************************************************************************************/
BidirectionalMotor::BidirectionalMotor(
    std::string name,   /**< [in] name of motor */
    uint16_t pwm_pin,
    uint16_t dir_pin
) : Motor(name, pwm_pin),
    dir_pin_id_(dir_pin)
{
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

common::ArcErrorCodes BidirectionalMotor::SetMotorCmd(
    int16_t cmd_pct     /**< [in] PWM command percentage and direction (-100 to 100) */
) {
    /* Really should have slew rate checks, but that is a TODO */
    if (cmd_pct > 100) {
        logger_->warn("motor cmd {} > 100", cmd_pct);
        cmd_pct = 100;
    } else if (cmd_pct < -100) {
        logger_->warn("motor cmd {} < -100", cmd_pct);
        cmd_pct = -100;
    }

    /* top bit is sign bit */
    if ((cmd_pct & 0x8000) != (prev_cmd_pct_ & 0x8000)) {
        /* direction of cmd needs to change
         *  top bit being set means it is negative
         */
        SetDirection( (cmd_pct & 0x8000) ? kMotorDirNegative : kMotorDirPositive );
    }

    prev_cmd_pct_ = cmd_pct;
    return common::kArcErrorNone; //SetPWM(abs(cmd_pct));
}

}}} //arc::arc::bsp