# A Serial Port Echo Project for the ULX3S Board

This is an example of a simple project which receives characters
over the FT231X USB serial port and echoes them back down the serial
port. The characters' bits are also shown on the LEDs.

To build the ULX3S bitstream, do a ```make ulx3s.bit``` . If you don't have
a 45F FPGA chip, change the ```nextpnr-ecp5 --45k``` line in the Makefile
to something more suitable. If you have an older board, you will need to
find one that matches your board. Have a look in
[https://github.com/emard/ulx3s/tree/master/doc/constraints](https://github.com/emard/ulx3s/tree/master/doc/constraints) .

The serial port runs at 115,200 bps. You will need to send ctrl-J in order to move down to the next line (instead of the usual Enter key). You will also need to disable
hardware flow control: with _minicom_, send ctrl-A O, Serial port setup, F, Exit.

The project also runs under Verilator 5: just do a `make` and the Verilator simulation will be built and start running. Hit ctrl-C to stop it. 
