#ifndef ARC_CONTROL_HH
#define ARC_CONTROL_HH

#include "motor.hh"

namespace arc { namespace arc {

class ControlSystem {
    public:
        ControlSystem();
        ~ControlSystem();

    private:
        bsp::BidirectionalMotor tail_rotor_;


        static const uint16_t kTailDirectionPin = 16;
        static const uint16_t kTailPwmPin = 26;
};

}} //arc::arc

#endif //ARC_CONTROL_HH