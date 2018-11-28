#ifndef ARC_CONTROL_HH
#define ARC_CONTROL_HH

#include "gnc_autopilot.hh"
#include "motor.hh"

namespace arc { namespace arc {

class ControlSystem {
 public:
  ControlSystem();
  ~ControlSystem();

  void InputDirectMotorCmds(
    rotor_cmds_t cmds
  );

 private:
  bsp::BidirectionalMotor tail_rotor_;

  static const uint16_t kTailDirectionPin = 16;
  static const uint16_t kTailPwmPin = 26;
};

}} //arc::arc

#endif //ARC_CONTROL_HH