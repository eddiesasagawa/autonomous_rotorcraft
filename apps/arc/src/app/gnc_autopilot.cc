#include "gnc_autopilot.hh"

namespace arc { namespace arc {

AutopilotBase::AutopilotBase(std::string name)
: logger_(common::Log::RetrieveLogger(name)),
  t_prev_(0),
  t_start_(0)
{

}

AutopilotBase::~AutopilotBase() {}

rotor_cmds_t AutopilotBase::Step(double t_now) {
  rotor_cmds_t cmd = {0,0,0};
  return cmd;
}

/*****************************************************************************************/
AutopilotSelfTest::AutopilotSelfTest()
: AutopilotBase("autopilot_test"),
  last_cmds_({0,0,0}),
  current_phase_(kSelfTestInitialPhase)
{

}

AutopilotSelfTest::~AutopilotSelfTest() {}

rotor_cmds_t AutopilotSelfTest::Step(double t_now) {
  rotor_cmds_t cmd = {0,0,0};

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
      cmd.pct_tail = 0;

      if (t_since_start >= kInitialPhaseDuration) {
        current_phase_ = kSelfTestUpperRotorPhase;
      }
      break;
    }

    case kSelfTestUpperRotorPhase: {
      break;
    }

    case kSelfTestLowerRotorPhase: {
      break;
    }

    case kSelfTestTailRotorPhase: {

    }

    case kSelfTestDone:
    default:
      // do nothing, leave zero commands
      break;
  }

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

}} // arc::arc