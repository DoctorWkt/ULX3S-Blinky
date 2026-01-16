.PHONY: all
all: obj_dir/Vecho
	obj_dir/Vecho

obj_dir/Vecho: echo.v txuartlite.v rxuartlite.v uartsim.cpp uartsim.h \
		echo_tb.cpp
	verilator -cc --exe --trace --build -j \
		echo.v txuartlite.v rxuartlite.v uartsim.cpp echo_tb.cpp

.PHONY: clean
clean:
	rm -rf obj_dir *.vcd
	rm -rf echo.json ulx3s_out.config ulx3s.bit

ulx3s.bit: ulx3s_out.config
	ecppack ulx3s_out.config ulx3s.bit

ulx3s_out.config: echo.json
	nextpnr-ecp5 --45k --json echo.json \
		--lpf ulx3s_v20.lpf \
		--package CABGA381 \
		--textcfg ulx3s_out.config 

echo.json: echo.ys echo.v
	yosys echo.ys 

prog: ulx3s.bit
	sudo ~wkt/.bin/ujprog *.bit
