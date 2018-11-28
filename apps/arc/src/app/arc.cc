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
    controller_() {
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

  int16_t tail_cmd = 0;

  /* main loop */
  while (!quit_now) {
    auto p_msg = a2hi_.RecvNonBlocking(&msg_recvd);

    if (msg_recvd) {
      switch(p_msg->MessageId()) {
        case common::kArcHostMsgIdCommand: {
          auto p_cmd_msg = std::static_pointer_cast<common::AHICommandMessage>(p_msg);

          switch (p_cmd_msg->command()) {
            case common::AHICommandMessage::kAhiCmdPitchForward: {
              if (tail_cmd < 100) {
                tail_cmd += 5;
              }
              if (tail_cmd > 100) {
                tail_cmd = 100;
              }
              logger_->info("updating tail cmd to {}", tail_cmd);
              controller_.InputDirectMotorCmds({0,0,tail_cmd});
              break;
            }

            case common::AHICommandMessage::kAhiCmdPitchBack: {
              if (tail_cmd > (-100)) {
                tail_cmd -= 5;
              }
              if (tail_cmd < -100) {
                tail_cmd = -100;
              }
              logger_->info("updating tail cmd to {}", tail_cmd);
              controller_.InputDirectMotorCmds({0,0,tail_cmd});
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