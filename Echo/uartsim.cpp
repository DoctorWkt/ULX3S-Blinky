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
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <ctype.h>

#include "uartsim.h"

UARTSIM::UARTSIM(void) {
	setup(25);	// Set us up for a baud rate of CLK/25
	m_rx_baudcounter = 0;
	m_tx_baudcounter = 0;
	m_rx_state = RXIDLE;
	m_tx_state = TXIDLE;
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
				m_tx_data = (-1<<10)
					// << nstart_bits
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
