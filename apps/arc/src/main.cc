/**
 * 
 * For now, just a simple test program for cross-compile testing
 * 
 */

#include <stdio.h>
#include <pigpio.h>

#include "arc2host_interface.hh"
#include "arc_host_msg.hh"

int main(int argc, char** argv) {
    if (argc != 5) {
        printf("usage: arc <MY_IP_ADDR> <MY_PORT> <HOST_IP_ADDR> <HOST_PORT>\n");
        return -1;
    }
    if (gpioInitialise() < 0) {
        printf("Failed to initialize gpio\n");
        return -1;
    }

    arc::arc::Arc2HostInterface& a2hi = arc::arc::Arc2HostInterface::GetInstance();
    a2hi.Connect(argv[1], argv[2], argv[3], argv[4]);
    
    bool quit_now = false;
    bool msg_recvd = false;

    while (!quit_now) {
        auto p_msg = a2hi.RecvNonBlocking(&msg_recvd);
        if (msg_recvd) {
            // printf("ARC received a message of type %d, yay!\n", p_msg->MessageId());
            switch(p_msg->MessageId()) {
                case arc::common::kArcHostMsgIdCommand: {
                    auto p_cmd_msg = std::static_pointer_cast<arc::common::AHICommandMessage>(p_msg);
                    printf("ARC received Command -- type: %d\n", p_cmd_msg->command());

                    quit_now = (p_cmd_msg->command() == arc::common::AHICommandMessage::kAhiCmdQuit);
                    break;
                }
                default:
                    printf("ARC received a message of type %d!\n", p_msg->MessageId());
                    break;
            }
        }
    }

    arc::common::AHIStatusMessage status;
    printf("Hello world from rpi pigpio.. \n");
    printf("ZMQ wrappers success! : %d \n", status.MessageId());

    gpioTerminate();
    return 0;
}
