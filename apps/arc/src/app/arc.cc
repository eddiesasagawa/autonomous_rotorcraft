#include "arc.hh"

#include <pigpio.h>
#include "spdlog/sinks/stdout_color_sinks.h"

#include "arc_host_msg.hh"

namespace arc { namespace arc {

Arc::Arc(
    const char* const my_ip_addr,      /**< [in] IP address that this side runs from */
    const char* const my_port,         /**< [in] Port that this side runs from */
    const char* const dest_ip_addr,     /**< [in] IP address of destination */
    const char* const dest_port        /**< [in] Port of destination */
) : a2hi_(Arc2HostInterface::GetInstance()),
    logger_(spdlog::stdout_color_st("console")) {
    /* Initialize ARC here */
    logger_->info("ARC -- initializing.");
    a2hi_.Connect(my_ip_addr, my_port, dest_ip_addr, dest_port);
}

Arc::~Arc() {
    /* Clean up ARC*/
    logger_->info("ARC -- terminating.");
}

void Arc::Spin() {
    bool quit_now = false;
    bool msg_recvd = false;

    /* main loop */
    while (!quit_now) {
        auto p_msg = a2hi_.RecvNonBlocking(&msg_recvd);

        if (msg_recvd) {
            switch(p_msg->MessageId()) {
                case common::kArcHostMsgIdCommand: {
                    auto p_cmd_msg = std::static_pointer_cast<common::AHICommandMessage>(p_msg);
                    printf("ARC received Command -- type: %d\n", p_cmd_msg->command());

                    quit_now = (p_cmd_msg->command() == common::AHICommandMessage::kAhiCmdQuit);
                    break;
                }

                default:
                    printf("ARC received a message of type %d!\n", p_msg->MessageId());
                    break;
            }
        }
    }

}

}} //arc::arc