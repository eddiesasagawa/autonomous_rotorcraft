#ifndef ARC_CONTROL_HH
#define ARC_CONTROL_HH

#include "motor.hh"
#include "arc_errors.hh"

namespace arc {
  namespace arc {

struct rotor_cmds_t {
  int16_t pct_upper;  /**< PWM command percentage (0-100) for main rotor (upper) */
  int16_t pct_lower;  /**< PWM command percentage (0-100) for main rotor (lower) */
  int16_t pct_tail;   /**< PWM command percentage and direction (-100 ~ 100) for tail rotor */
};

class Actuators {
 public:
  Actuators();
  ~Actuators();

  void InputDirectMotorCmds(
    rotor_cmds_t cmds
  );

 private:
  bsp::BidirectionalMotor tail_rotor_;
  bsp::UnidirectionalMotor upper_rotor_;
  bsp::UnidirectionalMotor lower_rotor_;

  static const uint16_t kTailDirectionPin = 26;
  static const uint16_t kTailPwmPin = 16;

  static const uint16_t kUpperRotorPwmPin = 18;
  static const uint16_t kLowerRotorPwmPin = 13;

  common::shared_log_ptr_t logger_;
};

  } //arc::arc
} // arc

#endif //ARC_CONTROL_HH