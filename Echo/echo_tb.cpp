// #include <verilatedos.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <signal.h>
#include "verilated.h"
#include "Vecho.h"
#include "testb.h"
#include "uartsim.h"

int	main(int argc, char **argv) {
	Verilated::commandArgs(argc, argv);
	TESTB<Vecho>	*tb
		= new TESTB<Vecho>;
        UARTSIM         *uart;
        unsigned        baudclocks;

        uart = new UARTSIM();
        baudclocks = tb->m_core->o_setup;
        uart->setup(baudclocks);

	tb->opentrace("echo.vcd");

	for (unsigned clocks=0; clocks < 20000000; clocks++) {
		tb->tick();
		tb->m_core->i_uart_rx = (*uart)(tb->m_core->o_uart_tx);
	}
	printf("\n\nSimulation complete\n");
}
