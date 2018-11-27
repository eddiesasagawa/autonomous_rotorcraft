#include "arc_control.hh"

namespace arc { namespace arc {

ControlSystem::ControlSystem()
: tail_rotor_("tail_rotor", kTailPwmPin, kTailDirectionPin) {

}

ControlSystem::~ControlSystem() {}

void ControlSystem::InputDirectMotorCmds(
    int16_t upper_rotor_pct,    /**< [in] PWM command percentage (0-100) for main rotor (upper) */
    int16_t lower_rotor_pct,    /**< [in] PWM command percentage (0-100) for main rotor (lower) */
    int16_t tail_rotor_pct      /**< [in] PWM command percentage and direction (-100 ~ 100) for tail rotor */
) {
    /* Command tail rotor */
    tail_rotor_.SetMotorCmd(tail_rotor_pct);
}

}}
