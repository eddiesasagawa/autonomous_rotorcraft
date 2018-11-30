#ifndef ARC_MOTOR_HH
#define ARC_MOTOR_HH

#include "gpio_base.hh"

namespace arc { namespace arc { namespace bsp {

/**
 * TODO(eddie) Add additional variables and logic for handling motor specs such as slew limits,
 * min/max limits, etc.
 */
class Motor : public GpioBase {
 public:
  Motor(
    std::string name,   /**< [in] name of motor */
    uint16_t pwm_pin    /**< [in] pin ID for PWM */
  );
  ~Motor();

  common::ArcErrorCodes SetPWM(
    uint16_t duty_cycle /**< [in] PWM duty cycle,  */
  );

  virtual common::ArcErrorCodes SetMotorCmd(
    int16_t cmd_pct     /**< [in] PWM command percentage (0 to 100) */
  );

 protected:
  uint16_t pwm_pin_id_;
  int16_t prev_cmd_pct_;
};

/**
 * Uni-Directional Motor (can only spin in one direction)
 */
class UnidirectionalMotor : public Motor {
 public:
  UnidirectionalMotor(
    std::string name,   /**< [in] name of motor */
    uint16_t pwm_pin /**< [in] pin ID for PWM */
  );
  ~UnidirectionalMotor();

};

/**
 * Bi-directional motor (can spin in either direction)
 */
class BidirectionalMotor : public Motor {
 public:
  enum Direction {
    kMotorDirPositive = 0,
    kMotorDirNegative = 1
  };

  BidirectionalMotor(
    std::string name,   /**< [in] name of motor */
    uint16_t pwm_pin,   /**< [in] pin ID for PWM output */
    uint16_t dir_pin    /**< [in] pin ID for direction output */
  );
  ~BidirectionalMotor();

  common::ArcErrorCodes SetDirection(Direction dir);
  common::ArcErrorCodes SetMotorCmd(
    int16_t cmd_pct     /**< [in] PWM command percentage and direction (-100 to 100) */
  ) override;

 private:
  uint16_t dir_pin_id_;
};

}}} //arc::arc::bsp

#endif //ARC_MOTOR_HH