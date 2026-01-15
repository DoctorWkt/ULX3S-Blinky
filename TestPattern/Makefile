##
## Verilator Rules
##
.PHONY: all
all: image.ppm

image.ppm: obj_dir/Vpattern
	obj_dir/Vpattern > image.ppm
	

obj_dir/Vpattern: pattern.v pattern_tb.cpp
	verilator -cc --exe --trace --build -j pattern.v pattern_tb.cpp

## 
.PHONY: clean
clean:
	rm -rf obj_dir image.ppm *.vcd
	rm -f ULX3S_45F.ys ULX3S_45F.json ulx3s_45f_ULX3S_45F.config \
		ulx3s_45f_ULX3S_45F.bit

##
## ULX3S Rules
##
YOSYS=yosys
NEXTPNR=nextpnr-ecp5
ECPPACK=ecppack
HDMICODE=DVI_3x3.v
HDMICODE=llhdmi.v vgatestsrc.v
ENCODER=TMDS_encoder.v

.PHONY: bitsrream
bitstream: ulx3s_45f_ULX3S_45F.bit

ulx3s_45f_ULX3S_45F.bit: ulx3s_45f_ULX3S_45F.config
	$(ECPPACK) --input ulx3s_45f_ULX3S_45F.config \
		--bit ulx3s_45f_ULX3S_45F.bit

ULX3S_45F.ys: ULX3S_45F.v $(HDMICODE) $(ENCODER) clock.v OBUFDS.v
	chmod +x ysgen.sh
	./ysgen.sh ULX3S_45F.v  $(HDMICODE) $(ENCODER) clock.v \
		OBUFDS.v  > ULX3S_45F.ys
	echo "hierarchy -top ULX3S_45F" >> ULX3S_45F.ys
	echo "synth_ecp5 -json ULX3S_45F.json" >> ULX3S_45F.ys

ULX3S_45F.json: ULX3S_45F.ys
	$(YOSYS) -q ULX3S_45F.ys

ulx3s_45f_ULX3S_45F.config: ULX3S_45F.json
	$(NEXTPNR) --45k --json ULX3S_45F.json --lpf ulx3s_v20_segpdi.lpf \
	--package CABGA381 \
	--textcfg ulx3s_45f_ULX3S_45F.config
