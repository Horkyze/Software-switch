A software switch with 2 ports.

DEPENDENCIES 

libpcap, C comliper (clang is default), make
computer with multiple NICs
administrative rights 

sudo apt-get install libpcap-dev clang make vim gdb

Run
$ make
to create a binary inside bin folder.

Optionaly run
$ make debug
to open gdb


EXAMPLE

sudo ./bin/s_switch -1 eth0 -2 wlan0


DOCUMENTATION

Up-to date documentation can be found here:
https://docs.google.com/document/d/1czP8rZtIDgggMDWtRk03onJesor6wTZoLFDI-oTcymY/edit?usp=sharing


CHANGE LOG

11 October 2015
	- application is able to list available interffaces
	- forwarding all frames from first interface to second 
		(only 30 bytes are forwarded, which makes the frames malformed, but still visible in wireshark)
