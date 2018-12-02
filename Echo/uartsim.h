////////////////////////////////////////////////////////////////////////////////
//
// Filename: 	uartsim.h
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
#ifndef	UARTSIM_H
#define	UARTSIM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <poll.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>

#define	TXIDLE	0
#define	TXDATA	1
#define	RXIDLE	0
#define	RXDATA	1

class	UARTSIM	{
	//
	// And the pieces of the setup register broken out.
	int	m_baud_counts;

	// UART state
	int	m_rx_baudcounter, m_rx_state, m_rx_bits,
		m_last_tx;
	int	m_tx_baudcounter, m_tx_state, m_tx_busy;
	unsigned	m_rx_data, m_tx_data;

public:
	//
	UARTSIM(void);

	// setup() adjusts the baud rate internally.  It should be called
	// once before starting.
	void	setup(unsigned isetup);

	// The operator() function is called on every tick.  The input is the
	// the output txuart transmit wire from the device.  The output is to
	// be connected to the the rxuart receive wire into the device.  This
	// makes hookup and operation very simple.
	//
	int	operator()(int i_tx);
};

#endif
