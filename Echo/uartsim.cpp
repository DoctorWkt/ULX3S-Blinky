////////////////////////////////////////////////////////////////////////////////
//
// Filename: 	uartsim.cpp
//
// Project:	Verilog Tutorial Example file
//
// Purpose:	A UART simulator, capable of interacting with a user over
//		stdin/stdout.
//
// Creator:	Dan Gisselquist, Ph.D.
//		Gisselquist Technology, LLC
//
////////////////////////////////////////////////////////////////////////////////
//
// Written and distributed by Gisselquist Technology, LLC
//
// This program is hereby granted to the public domain.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTIBILITY or
// FITNESS FOR A PARTICULAR PURPOSE.
//
////////////////////////////////////////////////////////////////////////////////
//
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <poll.h>
#include <termios.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <ctype.h>
#include <fcntl.h>

#include "uartsim.h"

// Original blocking terminal setting
struct termios orig_termios;

void reset_terminal_mode(void)
{
    tcsetattr(0, TCSANOW, &orig_termios);
}

void save_old_terminal_mode()
{
    tcgetattr(0, &orig_termios);
}

int ttySetCbreak(void)
{
    struct termios t;

    // Save the existing terminal mode
    // so that we can restore it on exit()
    save_old_terminal_mode();
    atexit(reset_terminal_mode);

    if (tcgetattr(0, &t) == -1)
        return -1;

    t.c_lflag &= ~(ICANON | ECHO);
    t.c_lflag |= ISIG;

    t.c_iflag &= ~ICRNL;

    t.c_cc[VMIN] = 0;                   /* Character-at-a-time input */
    t.c_cc[VTIME] = 0;                  /* with blocking */

    if (tcsetattr(0, TCSAFLUSH, &t) == -1)
        return -1;

    return 0;
}





UARTSIM::UARTSIM(void) {
	setup(25);	// Set us up for a baud rate of CLK/25
	m_rx_baudcounter = 0;
	m_tx_baudcounter = 0;
	m_rx_state = RXIDLE;
	m_tx_state = TXIDLE;

	// Put terminal into cbreak mode
  	ttySetCbreak();
}

void	UARTSIM::setup(unsigned isetup) {
	m_baud_counts = (isetup & 0x0ffffff);
}

int	UARTSIM::operator()(const int i_tx) {
	int	o_rx = 1, nr = 0;

	m_last_tx = i_tx;

	if (m_rx_state == RXIDLE) {
		if (!i_tx) {
			m_rx_state = RXDATA;
			m_rx_baudcounter =m_baud_counts+m_baud_counts/2-1;
			m_rx_bits    = 0;
			m_rx_data    = 0;
		}
	} else if (m_rx_baudcounter <= 0) {
		if (m_rx_bits >= 8) {
			m_rx_state = RXIDLE;
			putchar(m_rx_data);
			fflush(stdout);
		} else {
			m_rx_bits++;;
			m_rx_data = ((i_tx&1) ? 0x80 : 0)
				| (m_rx_data>>1);
		} m_rx_baudcounter = m_baud_counts-1;
	} else
		m_rx_baudcounter--;

	if (m_tx_state == TXIDLE) {
		struct	pollfd	pb;
		pb.fd = STDIN_FILENO;
		pb.events = POLLIN;
		if (poll(&pb, 1, 0) < 0)
			perror("Polling error:");

		if (pb.revents & POLLIN) {
			char	buf[1];

			nr = read(STDIN_FILENO, buf, 1);
			if (1 == nr) {
				m_tx_data = (-1<<9)	// One hi stop bit,
							// one low start bit
					|((buf[0]<<1)&0x01fe);
				m_tx_busy = (1<<(10))-1;
				m_tx_state = TXDATA;
				o_rx = 0;
				m_tx_baudcounter = m_baud_counts-1;
			} else if (nr < 0) {
				fprintf(stderr, "UARTSIM()::read() ERR\n");
			}
		}
	} else if (m_tx_baudcounter <= 0) {
		m_tx_data >>= 1;
		m_tx_busy >>= 1;
		if (!m_tx_busy)
			m_tx_state = TXIDLE;
		else
			m_tx_baudcounter = m_baud_counts-1;
		o_rx = m_tx_data&1;
	} else {
		m_tx_baudcounter--;
		o_rx = m_tx_data&1;
	}

	return o_rx;
}
