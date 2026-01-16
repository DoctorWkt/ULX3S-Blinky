# ULX3S HDMI Test Patterm Generator

I've taken the idea of a low-level video generator from Dan Gisselquist's
[llvga.v](https://github.com/ZipCPU/vgasim/blob/master/rtl/llvga.v) module
and also the
[HDMI code from Emard](https://github.com/DoctorWkt/Verilog_tic-tac-toe/tree/master/HDMI), and I've created a _llhdmi.v_ video generating module.

I have also taken Dan Gisselquist's
[vgatestsrc.v](https://github.com/ZipCPU/vgasim/blob/master/rtl/vgatestsrc.v)
test pattern generator, and built a ULX3S project that displays the test
pattern on the HDMI output.

To do this, ```make bistream``` to create the file
```ulx3s_45f_ULX3S_45F.bit```. The _dec_10_2018.jpg_
image shows a photo of the current HDMI output from the project.

There is also a make with Verilator: just do a ```make```. This uses the
test pattern generator and produces the image file ```image.ppm```.
