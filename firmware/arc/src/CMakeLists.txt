project(arc)

#set(RPI_SYSROOT $ENV{HOME}/raspberry-pi-tools/tools/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf/arm-linux-gnueabihf/sysroot)

# Add PiGPIO
# Include path to the pigpio .so file
set(PIGPIO_PATH $ENV{HOME}/raspberry-pi-tools/pigpio)
set(PIGPIO_BUILD_PATH ${PIGPIO_PATH}/build)

# Add the local 'include' directory and the pigpio directory to grab headers
include_directories(include ${PIGPIO_PATH})

# Actually find the pigpio library object
find_library(PIGPIO_LIB pigpio HINTS ${PIGPIO_BUILD_PATH} NO_CMAKE_FIND_ROOT_PATH)

# Alert the user if we do not find it
if(NOT PIGPIO_LIB)
 message(FATAL_ERROR "pigpio library not found")
endif()

#### CPPZMQ
#set(CPPZMQ_PATH $ENV{HOME}/raspberry-pi-tools/cppzmq)
#set(CPPZMQ_BUILD_PATH ${CPPZMQ_PATH}/build)
#include_directories(include ${CPPZMQ_PATH})
#find_package(cppzmq HINTS ${CPPZMQ_BUILD_PATH})

find_package(cppzmq)

if(NOT cppzmq_FOUND)
    message(FATAL_ERROR "cppzmq library not found")
endif()

# Executable
add_executable(arc main.cpp)
target_link_libraries(arc ${PIGPIO_LIB} cppzmq)
