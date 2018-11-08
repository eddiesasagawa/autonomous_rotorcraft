# This file is a general shell script for loading the correct config options to pass to ./configure for cross-compiling automake libraries for Raspberry Pi
# To use, copy this file to the source directory of the external library to compile for raspberry pi, and execute

TOOLCHAIN_HOST="arm-linux-gnueabihf"
TOOLCHAIN_ROOT_PATH="$HOME/raspberry-pi-tools/tools/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf"
TOOLCHAIN_PATH="${TOOLCHAIN_ROOT_PATH}/bin"

CPP="${TOOLCHAIN_PATH}/${TOOLCHAIN_HOST}-cpp"
CC="${TOOLCHAIN_PATH}/${TOOLCHAIN_HOST}-gcc"
CXX="${TOOLCHAIN_PATH}/${TOOLCHAIN_HOST}-g++"
LD="${TOOLCHAIN_PATH}/${TOOLCHAIN_HOST}-ld"
AS="${TOOLCHAIN_PATH}/${TOOLCHAIN_HOST}-as"
AR="${TOOLCHAIN_PATH}/${TOOLCHAIN_HOST}-ar"
RANLIB="${TOOLCHAIN_PATH}/${TOOLCHAIN_HOST}-ranlib"

SYSROOT=${TOOLCHAIN_ROOT_PATH}/${TOOLCHAIN_HOST}/sysroot
CFLAGS+="--sysroot=${SYSROOT}"
CPPFLAGS+="--sysroot=${SYSROOT}"
CXXFLAGS+="--sysroot=${SYSROOT}"

CONFIG_OPTS=()
CONFIG_OPTS+=("CFLAGS=${CFLAGS}")
CONFIG_OPTS+=("CPPFLAGS=${CPPFLAGS}") CONFIG_OPTS+=("CXXFLAGS=${CXXFLAGS}")
CONFIG_OPTS+=("LDFLAGS=${LDFLAGS}") CONFIG_OPTS+=("PKG_CONFIG_DIR=")
CONFIG_OPTS+=("--host=${TOOLCHAIN_HOST}")
CONFIG_OPTS+=("CPP=${CPP}")
CONFIG_OPTS+=("CC=${CC}")
CONFIG_OPTS+=("CXX=${CXX}")
CONFIG_OPTS+=("LD=${LD}")
CONFIG_OPTS+=("AS=${AS}")
CONFIG_OPTS+=("AR=${AR}")
CONFIG_OPTS+=("RANLIB=${RANLIB}")

# Set output directory
BUILD_PREFIX=$PWD/tmp
CONFIG_OPTS+=("--prefix=${BUILD_PREFIX}")

CONFIG_OPTS+=("PKG_CONFIG_DIR=")
CONFIG_OPTS+=("PKG_CONFIG_LIBDIR=${SYSROOT}/usr/lib/arm-linux-gnueabihf/pkgconfig:${SYSROOT}/usr/share/pkgconfig")
CONFIG_OPTS+=("PKG_CONFIG_SYSROOT=${SYSROOT}")

CONFIG_OPTS+=("PKG_CONFIG_PATH=${BUILD_PREFIX}/lib/pkgconfig")

# Now do the actual make process
pushd $1
{
    echo "Making $1"
    ./autogen.sh &&
    ./configure "${CONFIG_OPTS[@]}" &&
    make -j16 #&&
    #make install
} || exit 1
popd

