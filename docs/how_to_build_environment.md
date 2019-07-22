This How-To documents the process for setting up the build environment.

TODO .. in process

Raspberry Pi Zero Wireless
==========================
- Set up Raspbian for wireless ssh
- Configure wireless settings for extra security
- Run raspi-config to enable camera

Install gstreamer
-----------------
$ sudo apt-get install gstreamer1.0-tools
$ sudo apt-get install gstreamer1.0-plugins-bad
    - you could install plugins-base and plugins-good if you want..

Other libraries
---------------
- FOR NOW, having trouble getting static libraries to work, so still need libraries installed in raspbian:
- install pigpio:
$ cd <wherever/you/want>
$ git clone https://github.com/joan2937/pigpio.git
$ cd pigpio
$ mkdir build && cd build
$ sudo make install

- install ZeroMQ
$ cd <wherever/you/want>
$ git clone https://github.com/zeromq/libzmq.git
$ cd libzmq
$ mkdir build && cd build
$ sudo make install

$ cd <wherever/you/want>
$ git clone https://github.com/zeromq/cppzmq.git
$ cd cppzmq
$ mkdir build && cd build
$ sudo make install


Desktop (Ubuntu 18.04)
======================
- Install gstreamer dependencies
- Install OpenCV ...
- Install ZeroMQ

Setting up cross-compiling for Raspberry Pi
===========================================
Toolchain + pigpio set up
-------------------------
Follow instructions found at https://medium.com/@au42/the-useful-raspberrypi-cross-compile-guide-ea56054de187
except using pigpio instead of wiringPi
- In a separate directory for raspberry pi workspace (e.g. ~/raspberry-pi-tools/)
    - git clone https://github.com/raspberrypi/tools
    - git clone https://github.com/joan2937/pigpio
    - Copy the toolchain-rpi.cmake file from apps/arc directory to the pigpio directory
    - In the pigpio directory:
        - mkdir build && cd build
        - cmake .. -DCMAKE_TOOLCHAIN_FILE=../toolchain-rpi.cmake -DCMAKE_INSTALL_PREFIX=~/raspberry-pi-tools/tools/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf/arm-linux-gnueabihf/sysroot/usr
        - make
        The above should succeed and produce the .so files in the build folder

Set up ZeroMQ for Raspberry Pi
------------------------------
Took a little bit of work, but some references:
https://sappo.github.io/2016/10/13/Cross-compiling-for-the-Raspberry-Pi/
https://raspberrypi.stackexchange.com/questions/6761/cross-compiling-missing-packages
http://www.kaizou.org/2014/11/typical-cmake-project/

- I chose to clone to my raspberry-pi-tools folder (where tools and pigpio reside):
    $ cd ~/raspberry-pi-tools
- Clone libzmq:
    $ git clone https://github.com/zeromq/libzmq
- Clone cppzmq:
    $ git clone https://github.com/zeromq/cppzmq.git

- First install libzmq via cmake, per cppzmq build instructions:
    $ cd libzmq
    $ cp <path-to-this-repo>/apps/arc/toolchain-rpi.cmake ./toolchain-rpi.cmake
    $ mkdir build
    $ cd build
    $ cmake .. -DCMAKE_TOOLCHAIN_FILE=../toolchain-rpi.cmake -DCMAKE_INSTALL_PREFIX=~/raspberry-pi-tools/tools/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf/arm-linux-gnueabihf/sysroot/usr
    $ make install -j4

- Next install cppzmq by repeating the same steps
    $ cd ~/raspberry-pi-tools/cppzmq
    $ cp <path-to-this-repo>/apps/arc/toolchain-rpi.cmake ./toolchain-rpi.cmake
    $ mkdir build
    $ cd build
    $ cmake .. -DCMAKE_TOOLCHAIN_FILE=../toolchain-rpi.cmake -DCMAKE_INSTALL_PREFIX=~/raspberry-pi-tools/tools/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf/arm-linux-gnueabihf/sysroot/usr
    $ make install -j4

Set up spdlog for Raspberry Pi
------------------------------
Use spdlog for logging
$ cd ~/raspberry-pi-tools
$ git clone https://github.com/gabime/spdlog.git
$ cd spdlog
$ cp <path-to-this-repo>/apps/arc/toolchain-rpi.cmake ./toolchain-rpi.cmake
$ mkdir build && cd build
$ cmake .. -DCMAKE_TOOLCHAIN_FILE=../toolchain-rpi.cmake -DCMAKE_INSTALL_PREFIX=~/raspberry-pi-tools/tools/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf/arm-linux-gnueabihf/sysroot/usr -DSPDLOG_BUILD_BENCH=OFF -DSPDLOG_BUILD_TESTING=OFF -DSPDLOG_BUILD_EXAMPLES=OFF
$ make install -j4

Compiling ARC
=============
$ cd apps/arc
$ mkdir build && cd build
$ cmake .. -DCMAKE_TOOLCHAIN_FILE=../toolchain-rpi.cmake -DCMAKE_INSTALL_PREFIX=~/raspberry-pi-tools/tools/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf/arm-linux-gnueabihf/sysroot/usr
$ make
The compiled object 'arc' will reside in build/src

You should be able to just scp it over to the raspberry pi and execute it.
$ scp build/src/arc <rpiuser>@<rpi_ip_addr>:<destination/directory/arc>

Supported additional compile flags
----------------------------------
-DNO_ACTUATORS=1 : This will disable motor initialization and disable its use

To execute on Raspberry Pi
==========================
For now, since static libraries are not working, need to provide library path
Also need sudo for pigpio
$ sudo LD_LIBRARY_PATH=/usr/local/lib <path/to/arc>/arc

Compiling ARC-HOST
==================
$ cd apps/host
$ mkdir build && cd build (remove existing if exists)
$ cmake ..
$ make
The compiled object 'arc_host' will reside in build/src

To execute arc_host
-------------------
$ build/src/arc_host
