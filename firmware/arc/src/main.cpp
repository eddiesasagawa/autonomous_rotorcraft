/**
 * 
 * For now, just a simple test program for cross-compile testing
 * 
 */

#include <stdio.h>
#include <pigpio.h>
#include <zmq.hpp>

int main(int argc, char** argv) {

    if (gpioInitialise() < 0) {
        return -1;
    }

    zmq::context_t context;
    
    printf("Hello world from rpi pigpio.. \n");

    gpioTerminate();
    return 0;
}
