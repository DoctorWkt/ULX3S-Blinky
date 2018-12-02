`ifdef VERILATOR
module echo(i_clk, o_setup, o_led, o_uart_tx, i_uart_rx);
  input wire i_clk;
  output wire [31:0] o_setup;	// Tell UART co-sim about clocks per baud
  output reg [7:0] o_led;
  output wire o_uart_tx;    	// UART transmit signal line
  input  wire i_uart_rx;    	// UART receive signal line
`else
`include "txuartlite.v"
`include "rxuartlite.v"
module top(input clk_25mhz,
	   input ftdi_txd,
           output ftdi_rxd,
           output [7:0] led,
           output wifi_gpio0);
  wire o_uart_tx;    		// UART transmit signal line
  wire i_uart_rx;    		// UART receive signal line
  assign ftdi_rxd= o_uart_tx;	// Wire it up to real ULX3S line
  assign ftdi_txd= i_uart_rx;	// Wire it up to real ULX3S line
  reg [7:0] o_led;
  assign led= o_led;		// Wire the LEDs up to our register

  wire i_clk;
  assign i_clk= clk_25mhz;	// Wire our clock to the 25MHz ULX3S clock

  assign wifi_gpio0 = 1'b1;	// Tie GPIO0, keep board from rebooting
`endif

  /* verilator lint_off UNUSED */
  parameter CLOCK_RATE_HZ = 25_000_000;    // System clock rate in Hz
  parameter BAUD_RATE     =    115_200;    // Baud rate
  parameter CLOCKS_PER_BAUD = CLOCK_RATE_HZ/BAUD_RATE;
`ifdef VERILATOR
  assign o_setup = CLOCKS_PER_BAUD;
`endif

  // Interface to the UART
  reg  [7:0] tx_data;           // Data to send to the UART
  wire       tx_busy;           // Is it busy?
  reg        tx_stb;            // Strobe to ask to send data
  initial    tx_stb= 0;
  wire [7:0] rx_data;           // Each char typed by user
  wire rx_avail;                // If true, user data is available

  // Wire up the transmit and receive serial port modules
  txuartlite #(CLOCKS_PER_BAUD[23:0])
        transmitter(i_clk, tx_stb, tx_data, o_uart_tx, tx_busy);
  rxuartlite #(CLOCKS_PER_BAUD[23:0])
        receiver(i_clk, i_uart_rx, rx_avail, rx_data);
  /* verilator lint_on UNUSED */

  // Each time data is available, copy it to
  // tx_data and the LEDs, and strobe a transmission.
  // Turn off the strobe on the next data clock.
  always @(posedge i_clk) begin
    if (rx_avail && !tx_stb && !tx_busy) begin
      tx_data <= rx_data;
      o_led   <= rx_data;
      tx_stb  <= 1;
    end
    if (tx_stb) begin
      tx_stb  <= 0;
    end
  end

endmodule
