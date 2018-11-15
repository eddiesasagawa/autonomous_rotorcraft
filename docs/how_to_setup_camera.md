For this project, gstreamer is used to stream RPi camera feed to the host computer.

On the raspberry pi
===================
- Enable camera in raspi-config
- Make sure gstreamer is installed per how_to_build_environment.md

On the host PC
==============
- Make sure gstreamer is also installed

TESTING
=======
Reference: https://platypus-boats.readthedocs.io/en/latest/source/rpi/video/video-streaming-gstreamer.html

1. Run this command on the raspberry pi:
$ raspivid -fps 26 -h 450 -w 600 -vf -n -t 0 -b 200000 -o - | gst-launch-1.0 -v fdsrc ! h264parse ! rtph264pay config-interval=1 pt=96 ! gdppay ! tcpserversink host=YOUR_RPI_IP_ADDRESS port=YOUR_PORT_NUMBER

2. On the host PC, run this:
$ gst-launch-1.0 -v tcpclientsrc host=YOUR_RPI_IP_ADDRESS port=YOUR_PORT_NUMBER ! gdpdepay ! rtph264depay ! avdec_h264 ! videoconvert ! autovideosink sync=false

3. A window with the camera feed should open on the host PC.


