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

    if (gpioInitialise() < 0) {
        return -1;
    }

    arc::arc::Arc2HostInterface& a2h_if = arc::arc::Arc2HostInterface::GetInstance();
    arc::common::AHIStatusMessage status;

    printf("Hello world from rpi pigpio.. \n");
    printf("ZMQ wrappers success! : %d \n", status.MessageId());

    gpioTerminate();
    return 0;
}
