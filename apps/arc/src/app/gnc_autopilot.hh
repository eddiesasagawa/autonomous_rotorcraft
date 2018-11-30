#ifndef ARC_GNC_AUTOPILOT_HH
#define ARC_GNC_AUTOPILOT_HH

#include "arc_errors.hh"

namespace arc { namespace arc {

struct rotor_cmds_t {
  int16_t pct_upper;  /**< PWM command percentage (0-100) for main rotor (upper) */
  int16_t pct_lower;  /**< PWM command percentage (0-100) for main rotor (lower) */
  int16_t pct_tail;   /**< PWM command percentage and direction (-100 ~ 100) for tail rotor */
};

class AutopilotBase {
 public:
  AutopilotBase(std::string name);
  ~AutopilotBase();

  virtual rotor_cmds_t Step(double t_now);

 protected:
  double t_prev_;
  double t_start_;

 private:
  common::shared_log_ptr_t logger_;
};

/*****************************************************************************************/
/**
 * This autopilot class is intended to contain a self-test routine to test out rotor control.
 *
 * For now, this could just cycle through various pwm levels of each motor.
 * In the future though, this could be incorporated to something bigger where the accels could
 * also be read to verify that the expected forces and moments are being generated.
 *
 * Could even extend that to become calibration procedure..
 */
class AutopilotSelfTest : public AutopilotBase {
 public:
  AutopilotSelfTest();
  ~AutopilotSelfTest();

  rotor_cmds_t Step(double t_now) override;

 private:
  static const float kInitialPhaseDuration;
  static const int kInitialMainRotorEndCmd;      /**< 30% pwm at end of initial phase */
  static const int kUpperRotorDeltaCmdPerSec;
  static const int kLowerRotorDeltaCmdPerSec;
  static const int kTailRotorDeltaCmdPerSec;

  enum SelfTestPhase {
    kSelfTestInitialPhase = 0,
    kSelfTestUpperRotorPhaseUp,
    kSelfTestUpperRotorPhaseDown,
    kSelfTestLowerRotorPhaseUp,
    kSelfTestLowerRotorPhaseDown,
    kSelfTestTailRotorPhaseUp,
    kSelfTestTailRotorPhaseDown,
    kSelfTestReset,
    kSelfTestDone
  };

  SelfTestPhase current_phase_;
  rotor_cmds_t last_cmds_;
};

/*****************************************************************************************/
class AutopilotTactical : public AutopilotBase {
 public:
  AutopilotTactical();
  ~AutopilotTactical();

  rotor_cmds_t Step(double t_now) override;
};

}} // arc::arc

#endif //ARC_GNC_AUTOPILOT_HH