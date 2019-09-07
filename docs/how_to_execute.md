To start ARC on raspberry pi:
=============================
copy apps/arc/start_arc.sh to raspberry pi
copy apps/arc/build/src/arc to raspberry pi

start using:
$ start_arc.sh MY_RPI_IP_ADDRESS PORT_NUMBER

Pro-tip:
--------
Use ifconfig to find your IP on your network
Then use nmap 10.0.0.0/24 to find all devices on this network

