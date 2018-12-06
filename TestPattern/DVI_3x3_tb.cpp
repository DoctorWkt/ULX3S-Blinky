#include <verilatedos.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <signal.h>
#include "verilated.h"
#include "VDVI_3x3.h"
#include "testb.h"

int	main(int argc, char **argv) {
	Verilated::commandArgs(argc, argv);
	TESTB<VDVI_3x3>	*tb
		= new TESTB<VDVI_3x3>;

	tb->opentrace("DVI_3x3.vcd");
	tb->m_core->in_red= 0xf0;
	tb->m_core->in_green= 0x80;
	tb->m_core->in_blue= 0x20;

	for (unsigned clocks=0; clocks < 4400000; clocks++) {
		tb->tick();
	}
	printf("\n\nSimulation complete\n");
}
