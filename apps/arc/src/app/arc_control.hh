#ifndef ARC_CONTROL_HH
#define ARC_CONTROL_HH

#include "motor.hh"

namespace arc { namespace arc {

class ControlSystem {
    public:
        ControlSystem();
        ~ControlSystem();

        void InputDirectMotorCmds(
            int16_t upper_rotor_pct,    /**< [in] PWM command percentage (0-100) for main rotor (upper) */
            int16_t lower_rotor_pct,    /**< [in] PWM command percentage (0-100) for main rotor (lower) */
            int16_t tail_rotor_pct      /**< [in] PWM command percentage and direction (-100 ~ 100) for tail rotor */
        );

    private:
        bsp::BidirectionalMotor tail_rotor_;

        static const uint16_t kTailDirectionPin = 16;
        static const uint16_t kTailPwmPin = 26;
};

}} //arc::arc

#endif //ARC_CONTROL_HH