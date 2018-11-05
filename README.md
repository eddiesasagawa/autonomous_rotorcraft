# autonomous_rotorcraft
AutonomousRotorCraft (ARC) project

This is a little side project on adding autonomy to a cheap RC helicopter (17" length).

Intent is to have a visual-inertial navigation system and eventually try a bit of SLAM.
Due to possible resource constraints on the onboard processor, visual processing will be done on a desktop computer via streaming, and provide the onboard processor with visual updates.

Since I am most proficient in C and Python, I thought I would switch things up and do the bulk of the software in C++.

Apps
====
There are two main software targets:

ARC
---
This is the software that will reside on the RC copter directly, handling state estimation and ARC control

SERVER
------
This is the application to reside on the desktop PC that will be connected to ARC. It will receive a video stream from ARC and provide visual processing. There will also be a command/response interface link for passing command overrides and receiving online state estimate updates.

Hardware
========
- Raspberry Pi Zero Wireless
- Raspberry Pi Camera
- 6-DOF IMU
- 3-DOF Magnetometer
- MOSFETs and H-bridges for driving the 3 rotors
- Power: DC boost converter to run off 3.7V battery to maintain compatibility with current motor specs
- 3.7V, 2500 mAh LiPo battery (upgrade from 1200mAh that came with copter)

Software
========
- Raspbian Lite on the Raspberry Pi Zero
- Ubuntu 18.04 on desktop computer

3rd Party Libraries
-------------------
- ZeroMQ (C++)
- PiGPIO
- OpenCV

Some Resource Links
===================
Streaming raspberry pi video via gstreamer:
http://www.raspberry-projects.com/pi/pi-hardware/raspberry-pi-camera/streaming-video-using-gstreamer

However, there seem to be better options for gstreamer in this thread, so that it is UDP not TCP:
https://stackoverflow.com/questions/46219454/how-to-open-a-gstreamer-pipeline-from-opencv-with-videowriter

Cross-compiling for raspberry pi:
https://medium.com/@au42/the-useful-raspberrypi-cross-compile-guide-ea56054de187


