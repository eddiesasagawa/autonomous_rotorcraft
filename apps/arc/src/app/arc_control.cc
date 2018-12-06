#include "arc_control.hh"

namespace arc {
  namespace arc {

ControlSystem::ControlSystem()
: tail_rotor_("tail_rotor", kTailPwmPin, kTailDirectionPin),
  upper_rotor_("upper_rotor", kUpperRotorPwmPin),
  lower_rotor_("lower_rotor", kLowerRotorPwmPin),
  logger_(common::Log::RetrieveLogger("ctl_sys"))
{

}

ControlSystem::~ControlSystem() {}

void ControlSystem::InputDirectMotorCmds(rotor_cmds_t cmds) {
  logger_->info("cmd update: upper={:d}, lower={:d}, tail={:d}", cmds.pct_upper, cmds.pct_lower, cmds.pct_tail);

  tail_rotor_.SetMotorCmd(cmds.pct_tail);
  upper_rotor_.SetMotorCmd(cmds.pct_upper);
  lower_rotor_.SetMotorCmd(cmds.pct_lower);
}

  } // arc::arc
} //arc
