#include "arc.hh"

#include <pigpio.h>

#include "arc_host_msg.hh"

namespace arc { namespace arc {

const std::string Arc::kLogName = "arc";

Arc::Arc(
  const char* const my_ip_addr,      /**< [in] IP address that this side runs from */
  const char* const my_port,         /**< [in] Port that this side runs from */
  const char* const dest_ip_addr,     /**< [in] IP address of destination */
  const char* const dest_port        /**< [in] Port of destination */
) : a2hi_(Arc2HostInterface::GetInstance()),
    logger_(common::Log::RetrieveLogger(kLogName)),
    autopilot_(new AutopilotBase()),
    imu_(bsp::ST_6DOFImu_LSM6DS33()) {
  /* Initialize ARC here */
  logger_->info("initializing.");
  a2hi_.Connect(my_ip_addr, my_port, dest_ip_addr, dest_port);
}

Arc::~Arc() {
  /* Clean up ARC*/
  logger_->info("terminating.");
}

void Arc::Spin() {
  bool quit_now = false;
  bool msg_recvd = false;
  rotor_cmds_t cmds = {0,0,0};

  auto UpdateCmd = [](int16_t &cmd_part, int16_t step) {
    cmd_part += step;
    if (cmd_part < -100) { cmd_part = -100; }
    else if (cmd_part > 100) { cmd_part = 100; }
  };

  /* main loop */
  while (!quit_now) {
    auto p_msg = a2hi_.RecvNonBlocking(&msg_recvd);

    if (msg_recvd) {
      switch(p_msg->MessageId()) {
        case common::kArcHostMsgIdCommand: {
          auto p_cmd_msg = std::static_pointer_cast<common::AHICommandMessage>(p_msg);

          switch (p_cmd_msg->command()) {
            case common::AHICommandMessage::kAhiCmdStatus: {
              imu_.Query();
              break;
            }

            case common::AHICommandMessage::kAhiCmdPitchForward: {
              UpdateCmd(cmds.pct_tail, 10);
              autopilot_->BypassToMotors(cmds);
              break;
            }

            case common::AHICommandMessage::kAhiCmdPitchBack: {
              UpdateCmd(cmds.pct_tail, -10);
              autopilot_->BypassToMotors(cmds);
              break;
            }

            case common::AHICommandMessage::kAhiCmdUpperRotorUp: {
              UpdateCmd(cmds.pct_upper, 10);
              autopilot_->BypassToMotors(cmds);
              break;
            }

            case common::AHICommandMessage::kAhiCmdUpperRotorDown: {
              UpdateCmd(cmds.pct_upper, -10);
              autopilot_->BypassToMotors(cmds);
              break;
            }

            case common::AHICommandMessage::kAhiCmdLowerRotorUp: {
              UpdateCmd(cmds.pct_lower, 10);
              autopilot_->BypassToMotors(cmds);
              break;
            }

            case common::AHICommandMessage::kAhiCmdLowerRotorDown: {
              UpdateCmd(cmds.pct_lower, -10);
              autopilot_->BypassToMotors(cmds);
              break;
            }

            case common::AHICommandMessage::kAhiCmdQuit: {
              quit_now = true;
              break;
            }

            default: {
              logger_->info("ARC received command -- type: {:d}", p_cmd_msg->command());
              break;
            }
          }

          break;
        }

        default: {
          logger_->info("ARC received a message of type {:d}", p_msg->MessageId());
          break;
        }
      }
    }
  }
}

}} //arc::arc