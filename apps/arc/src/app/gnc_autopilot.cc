#include "gnc_autopilot.hh"

namespace arc {
  namespace arc {

AutopilotBase::AutopilotBase()
: t_prev_(0),
  t_start_(0),
  actuators_(),
  logger_(common::Log::RetrieveLogger("autopilot"))
{

}

AutopilotBase::AutopilotBase(std::string name)
: t_prev_(0),
  t_start_(0),
  actuators_(),
  logger_(common::Log::RetrieveLogger(name))
{

}

AutopilotBase::~AutopilotBase() {}

rotor_cmds_t AutopilotBase::Step(double t_now) {
  rotor_cmds_t cmd = {0,0,0};
  return cmd;
}

void AutopilotBase::BypassToMotors(rotor_cmds_t cmds) {
  actuators_.InputDirectMotorCmds(cmds);
}

/*****************************************************************************************/
const float AutopilotSelfTest::kInitialPhaseDuration = 5.0;
const int AutopilotSelfTest::kInitialMainRotorEndCmd = 20;      /**< 30% pwm at end of initial phase */
const int AutopilotSelfTest::kUpperRotorDeltaCmdPerSec = 10;
const int AutopilotSelfTest::kLowerRotorDeltaCmdPerSec = 10;
const int AutopilotSelfTest::kTailRotorDeltaCmdPerSec = 20;

AutopilotSelfTest::AutopilotSelfTest()
: AutopilotBase("autopilot_test"),
  current_phase_(kSelfTestInitialPhase),
  last_cmds_({0,0,0})
{

}

AutopilotSelfTest::~AutopilotSelfTest() {}

rotor_cmds_t AutopilotSelfTest::Step(double t_now) {
  rotor_cmds_t cmd = last_cmds_;

  if (0 == t_prev_) {
    t_prev_ = t_now;
    t_start_ = t_now;
  }

  double dt = t_now - t_prev_;  // will be zero on first step, watch for divide by zeros

  switch (current_phase_) {
    case kSelfTestInitialPhase: {
      /* In this phase, ramp main motors up to some small command to get them started */
      double t_since_start = t_now - t_start_;
      cmd.pct_upper = t_since_start * (kInitialMainRotorEndCmd / kInitialPhaseDuration);
      cmd.pct_lower = cmd.pct_upper;

      if (t_since_start >= kInitialPhaseDuration) {
        current_phase_ = kSelfTestUpperRotorPhaseUp;
      }
      break;
    }

    case kSelfTestUpperRotorPhaseUp: {
      cmd.pct_upper = last_cmds_.pct_upper + dt*kUpperRotorDeltaCmdPerSec;

      if (cmd.pct_upper >= 100) {
        cmd.pct_upper = 100;
        current_phase_ = kSelfTestUpperRotorPhaseDown;
      }
      break;
    }

    case kSelfTestUpperRotorPhaseDown: {
      cmd.pct_upper = last_cmds_.pct_upper - dt*kUpperRotorDeltaCmdPerSec;

      if (cmd.pct_upper <= kInitialMainRotorEndCmd) {
        cmd.pct_upper = kInitialMainRotorEndCmd;
        current_phase_ = kSelfTestLowerRotorPhaseUp;
      }
      break;
    }

    case kSelfTestLowerRotorPhaseUp: {
      cmd.pct_lower = last_cmds_.pct_lower + dt*kLowerRotorDeltaCmdPerSec;

      if (cmd.pct_lower >= 100) {
        cmd.pct_lower = 100;
        current_phase_ = kSelfTestLowerRotorPhaseDown;
      }
      break;
    }

    case kSelfTestLowerRotorPhaseDown: {
      cmd.pct_lower = last_cmds_.pct_lower - dt*kLowerRotorDeltaCmdPerSec;

      if (cmd.pct_lower <= kInitialMainRotorEndCmd) {
        cmd.pct_lower = kInitialMainRotorEndCmd;
        current_phase_ = kSelfTestTailRotorPhaseUp;
      }
      break;
    }

    case kSelfTestTailRotorPhaseUp: {
      cmd.pct_tail = last_cmds_.pct_tail + dt*kTailRotorDeltaCmdPerSec;

      if (cmd.pct_tail >= 100) {
        cmd.pct_tail = 100;
        current_phase_ = kSelfTestTailRotorPhaseDown;
      }
      break;
    }

    case kSelfTestTailRotorPhaseDown: {
      cmd.pct_tail = last_cmds_.pct_tail - dt*kTailRotorDeltaCmdPerSec;

      if (cmd.pct_tail <= -100) {
        cmd.pct_tail = 100;
        current_phase_ = kSelfTestReset;
      }
      break;
    }

    case kSelfTestReset: {
      cmd.pct_tail = last_cmds_.pct_tail + dt*kTailRotorDeltaCmdPerSec;
      cmd.pct_upper = last_cmds_.pct_upper - dt*kUpperRotorDeltaCmdPerSec;
      cmd.pct_lower = last_cmds_.pct_lower - dt*kLowerRotorDeltaCmdPerSec;

      if ((cmd.pct_tail == 0) && (cmd.pct_lower <= 0) &&(cmd.pct_upper <= 0)) {
        cmd.pct_upper = 0;
        cmd.pct_lower = 0;
        current_phase_ = kSelfTestDone;
      }
      break;
    }

    case kSelfTestDone:
    default:
      // do nothing, leave zero commands
      break;
  }

  last_cmds_ = cmd;
  return cmd;
}

/*****************************************************************************************/
AutopilotTactical::AutopilotTactical()
: AutopilotBase("autopilot_tac")
{

}

AutopilotTactical::~AutopilotTactical() {}

rotor_cmds_t AutopilotTactical::Step(double t_now) {
  rotor_cmds_t cmd = {0,0,0};

  return cmd;
}

  } // arc::arc
} // arc