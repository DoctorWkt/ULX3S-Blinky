# A Working Blinky for The ULX3S Board

I just got a blinky project working for my new ULX3S Board with a 45F FPGA
chip. I had to git clone and rebuild all three of _yosys_, _trellis_ and
_nextpnr_. Details of doing this are at https://github.com/YosysHQ/yosys
and https://github.com/YosysHQ/nextpnr#nextpnr-ecp5 .

The latest version of _nextpnr_ supports an external constraints file.
I have a v3.0.3 ULX3S board, and the constraints file for this is
```ulx3s_v20.lpf```.

The ```blinky.v``` file can be simulated under Verilator, just type ```make```
at the command line. It will use ```blinky_tb.cpp``` as the testbench and
that requires ```testb.h```.

To build the ULX3S bitstream, do a ```make ulx3s.bit``` . If you don't have
a 45F FPGA chip, change the ```nextpnr-ecp5 --45k``` line in the Makefile
to something more suitable. If you have an older board, you will need to
find one that matches your board. Have a look in
https://github.com/emard/ulx3s/tree/master/doc/constraints .

Making the ```ulx3s.bit``` file requires ```blinky.ys``` which tells
_yosys_ what to do, and ```ulx3s_v20.lpf``` and ```ulx3s_empty.config```
to tell _nextpnr_ what to do.

Use ```ujprog``` from [https://github.com/emard/ulx3s-bin] to program
your board.
