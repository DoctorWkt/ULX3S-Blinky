.PHONY: all
all: blinky
	echo "Running blinky simulation ..."
	./blinky
	echo "Now look at the blinky.vcd output file"

blinky: blinky.v blinky_tb.cpp
	verilator -cc --exe --trace --build -j blinky.v blinky_tb.cpp
	cp obj_dir/Vblinky blinky

.PHONY: clean
clean:
	rm -rf blinky obj_dir *.vcd
	rm -rf blinky.json ulx3s_out.config ulx3s.bit

ulx3s.bit: ulx3s_out.config
	ecppack ulx3s_out.config ulx3s.bit

ulx3s_out.config: blinky.json
	nextpnr-ecp5 --45k --json blinky.json \
		--package CABGA381 \
		--lpf ulx3s_v20.lpf \
		--textcfg ulx3s_out.config 

blinky.json: blinky.ys blinky.v
	yosys blinky.ys 

ujprog: ulx3s.bit
	ujprog *.bit
