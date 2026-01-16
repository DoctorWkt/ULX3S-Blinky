# A Blinky Project for the ULX3S Board

Here is a blinky project for the ULX3S board with a 45F FPGA.

I downloaded the latest version of _Yosys_ from
[https://github.com/YosysHQ/oss-cad-suite-build/releases/latest](https://github.com/YosysHQ/oss-cad-suite-build/releases/latest).

The ```blinky.v``` file can be simulated under Verilator, just type ```make```
at the command line. It will use ```blinky_tb.cpp``` as the testbench and
that requires ```testb.h```.

To build the ULX3S bitstream, do a ```make ulx3s.bit``` . If you don't have
a 45F FPGA chip, change the ```nextpnr-ecp5 --45k``` line in the Makefile
to something more suitable. If you have an older board, you will need to
find a constraints file that matches your board. Have a look in
[https://github.com/emard/ulx3s/tree/master/doc/constraints](https://github.com/emard/ulx3s/tree/master/doc/constraints) .

Making the ```ulx3s.bit``` file requires ```blinky.ys``` which tells
_Yosys_ what to do, and ```ulx3s_v20.lpf```
to tell _nextpnr_ what to do.

Use ```ujprog``` from [https://github.com/emard/ulx3s-bin](https://github.com/emard/ulx3s-bin) to program
your board.

## Other Projects and Tips

If you are using the serial link to the FT231X chip, you need to disable
hardware flow control. With _minicom_, ctrl-A O, Serial port setup, F, Exit.

As I get other projects to work on the board, I'll put links below:

  * [Echo characters](Echo/) on the USB serial port
  * [Tic Tac Toe](https://github.com/DoctorWkt/Verilog_tic-tac-toe) using the serial port, and with HDMI output
  * [A 4-bit CPU](https://github.com/DoctorWkt/CSCv2/tree/master/Verilog) using the serial port
  * [A HDMI test pattern generator](TestPattern/) based on Emard and Dan Gisselquist's code
