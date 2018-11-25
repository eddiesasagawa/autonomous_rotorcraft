project(arc)

set(RPI_SYSROOT $ENV{HOME}/raspberry-pi-tools/tools/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf/arm-linux-gnueabihf/sysroot)

# Add PiGPIO
# Include path to the pigpio .so file
set(PIGPIO_PATH $ENV{HOME}/raspberry-pi-tools/pigpio)
set(PIGPIO_BUILD_PATH ${PIGPIO_PATH}/build)

# Add the local 'include' directory and the pigpio directory to grab headers
include_directories(include ${PIGPIO_PATH})

# Actually find the pigpio library object
find_library(PIGPIO_LIB pigpio HINTS ${PIGPIO_BUILD_PATH} NO_CMAKE_FIND_ROOT_PATH)
# find_package(pigpio HINTS ${PIGPIO_BUILD_PATH})

# Alert the user if we do not find it
if(NOT PIGPIO_LIB)
 message(STATUS "pigpio library not found")
endif()

#### CPPZMQ
set(CPPZMQ_PATH $ENV{HOME}/raspberry-pi-tools/cppzmq)
set(CPPZMQ_BUILD_PATH ${CPPZMQ_PATH}/build)
include_directories(include ${CPPZMQ_PATH})
find_package(cppzmq HINTS ${CPPZMQ_BUILD_PATH})

# include_directories(include ${RPI_SYSROOT}/usr/include)
# find_package(cppzmq PATHS ${RPI_SYSROOT}/usr/share/cmake/cppzmq)

if(NOT cppzmq_FOUND)
    message(STATUS "cppzmq library not found")
endif()

#### SPDLOG
set(SPDLOG_PATH $ENV{HOME}/raspberry-pi-tools/spdlog)
set(SPDLOG_BUILD_PATH ${SPDLOG_PATH}/build)
set(SPDLOG_BUILD_BENCH OFF)
include_directories(include ${SPDLOG_PATH}/include/spdlog)
add_subdirectory(${SPDLOG_PATH} ${CMAKE_CURRENT_BINARY_DIR}/spdlog)

# include_directories(include ${SPDLOG_PATH})
# find_package(spdlog)
# include_directories(include ${spdlog_LIB})

## Project sources
# library headers and sources
set(ARC_COMMON_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../common)
message(STATUS "Showing current source: ${CMAKE_CURRENT_SOURCE_DIR}")

include_directories(
    ${ARC_COMMON_DIR}/interfaces
    ${ARC_COMMON_DIR}/msg
)
set(ARC_COMMON_SRC 
    ${ARC_COMMON_DIR}/interfaces/arc_host_interface.cc 
    ${ARC_COMMON_DIR}/interfaces/arc_errors.cc
    ${ARC_COMMON_DIR}/msg/arc_host_msg.cc
)

# ARC sources
set(ARC_SRC_APP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/app)
set(ARC_SRC_APP
    ${ARC_SRC_APP_DIR}/arc.cc
    ${ARC_SRC_APP_DIR}/arc2host_interface.cc
)

set(ARC_SRC_BSP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bsp)
set(ARC_SRC_BSP
)

include_directories(
    ${ARC_SRC_APP_DIR}
    ${ARC_SRC_BSP_DIR}
)

# compiler/linker flags
# message(STATUS "Using sysroot: ${CMAKE_SYSROOT}")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --sysroot=${CMAKE_SYSROOT}" CACHE INTERNAL "" FORCE)
# set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} --sysroot=${CMAKE_SYSROOT}" CACHE INTERNAL "" FORCE)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --sysroot=${CMAKE_SYSROOT}" CACHE INTERNAL "" FORCE)
# set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} --sysroot=${CMAKE_SYSROOT}" CACHE INTERNAL "" FORCE)
# add_definitions("--sysroot=${CMAKE_SYSROOT}")
# set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --sysroot=${CMAKE_SYSROOT}" CACHE INTERNAL "" FORCE)
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --sysroot=${CMAKE_SYSROOT}" CACHE INTERNAL "" FORCE)
# set(CMAKE_C_COMPILE_OPTIONS_SYSROOT "--sysroot=${CMAKE_SYSROOT}")
# set(CMAKE_CXX_COMPILE_OPTIONS_SYSROOT "--sysroot=${CMAKE_SYSROOT}")
# set(CFLAGS "${CFLAGS} --sysroot=${CMAKE_SYSROOT}")

# include_directories(BEFORE SYSTEM ${RPI_SYSROOT}/usr/include)
# include_directories(BEFORE SYSTEM ${RPI_SYSROOT}/../include/c++/4.9.3)

# Executable
add_executable(arc
    main.cc
    ${ARC_COMMON_SRC}
    ${ARC_SRC_APP}
    ${ARC_SRC_BSP}
)
target_compile_features(arc PUBLIC cxx_std_11)
# target_compile_definitions(arc PRIVATE ZMQ_STATIC)

target_link_libraries(arc ${PIGPIO_LIB} cppzmq spdlog)
