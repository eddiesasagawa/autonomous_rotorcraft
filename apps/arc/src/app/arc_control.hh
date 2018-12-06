#ifndef ARC_CONTROL_HH
#define ARC_CONTROL_HH

#include "gnc_autopilot.hh"
#include "motor.hh"
#include "arc_errors.hh"

namespace arc {
  namespace arc {

class ControlSystem {
 public:
  ControlSystem();
  ~ControlSystem();

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