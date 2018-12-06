# ULX3S HDMI Test Patterm Generator

I've taken the idea of a low-level video generator from Dan Gisselquist's
[llvga.v](https://github.com/ZipCPU/vgasim/blob/master/rtl/llvga.v) module
and also the
[HDMI code from Emard](https://github.com/DoctorWkt/Verilog_tic-tac-toe/tree/master/HDMI), and I've created a _llhdmi.v_ video generating module.

I have also taken Dan Gisselquist's
[vgatestsrc.v](https://github.com/ZipCPU/vgasim/blob/master/rtl/vgatestsrc.v)
test pattern generator, and built a ULX3S project that displays the test
pattern on the HDMI output.

To do this, ```make -f UMakefile```.

There is also a make with Verilator, ```make -f Makefile```. This uses the
test pattern generator and produces the image file ```image.ppm```.

Also, as at 6th Dec 2018, this is rough code. It works as of half an hour
ago. I do need to tidy it up, document it more and add formal verification.

The dec_6_2018.jpg image is a photo of the first output from the project.
