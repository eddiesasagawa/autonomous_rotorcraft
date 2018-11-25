#include "arc_control.hh"

namespace arc { namespace arc {

ControlSystem::ControlSystem()
: tail_rotor_(kTailPwmPin, kTailDirectionPin) {

}

ControlSystem::~ControlSystem() {}

}}