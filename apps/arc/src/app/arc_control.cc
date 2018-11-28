#include "arc_control.hh"

namespace arc { namespace arc {

ControlSystem::ControlSystem()
: tail_rotor_("tail_rotor", kTailPwmPin, kTailDirectionPin) {

}

ControlSystem::~ControlSystem() {}

void ControlSystem::InputDirectMotorCmds(rotor_cmds_t cmds) {
    /* Command tail rotor */
    tail_rotor_.SetMotorCmd(cmds.pct_tail);
}

}}
