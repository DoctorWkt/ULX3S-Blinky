`default_nettype none
`ifdef VERILATOR
module DVI_3x3(
  input pixclk, // 25MHz
  input shiftclk, // 250MHz
  input [7:0] in_red, in_green, in_blue,
  output wire o_red,
  output wire o_grn,
  output wire o_blu
);
`else
module DVI_3x3(
  input pixclk, // 25MHz
  input shiftclk, // 250MHz
  input [7:0] in_red, in_green, in_blue,
  output wire o_red,
  output wire o_grn,
  output wire o_blu
  //output [2:0] TMDSp, TMDSn,
  //output TMDSp_clock, TMDSn_clock
);
`endif

  wire  clk_TMDS;
  assign clk_TMDS = shiftclk;

  reg [9:0] CounterX, CounterY;
  reg hSync, vSync, DrawArea;
  always @(posedge pixclk)
    DrawArea <= (CounterX<640) && (CounterY<480);

always @(posedge pixclk)
    CounterX <= (CounterX==799) ? 0 : CounterX+1;

always @(posedge pixclk)
  if (CounterX==799)
    CounterY <= (CounterY==524) ? 0 : CounterY+1;

always @(posedge pixclk)
  hSync <= (CounterX>=656) && (CounterX<752);

always @(posedge pixclk)
  vSync <= (CounterY>=490) && (CounterY<492);

  wire [9:0] TMDS_red, TMDS_green, TMDS_blue;
  TMDS_encoder encode_R(.clk(pixclk), .VD(in_red), .CD(2'b00), .VDE(DrawArea),
	.TMDS(TMDS_red));
  TMDS_encoder encode_G(.clk(pixclk), .VD(in_green), .CD(2'b00), .VDE(DrawArea),
	.TMDS(TMDS_green));
  TMDS_encoder encode_B(.clk(pixclk), .VD(in_blue), .CD({vSync,hSync}),
	.VDE(DrawArea), .TMDS(TMDS_blue));

  reg [3:0] TMDS_mod10=0; // modulus 10 counter
  reg [9:0] TMDS_shift_red=0, TMDS_shift_green=0, TMDS_shift_blue=0;
  reg TMDS_shift_load=0;
  always @(posedge clk_TMDS)
    TMDS_shift_load <= (TMDS_mod10==4'd9);

  always @(posedge clk_TMDS) begin
/* verilator lint_off WIDTH */
    TMDS_shift_red <= TMDS_shift_load ? TMDS_red : TMDS_shift_red [9:1];
    TMDS_shift_green <= TMDS_shift_load ? TMDS_green : TMDS_shift_green[9:1];
    TMDS_shift_blue <= TMDS_shift_load ? TMDS_blue : TMDS_shift_blue [9:1];	
    TMDS_mod10 <= (TMDS_mod10==4'd9) ? 4'd0 : TMDS_mod10+4'd1;
/* verilator lint_on WIDTH */
  end

`ifdef VERILATOR
  assign o_red= TMDS_shift_red[0];
  assign o_grn= TMDS_shift_green[0];
  assign o_blu= TMDS_shift_blue[0];
`else
  //OBUFDS OBUFDS_red (.I(TMDS_shift_red [0]), .O(TMDSp[2]), .OB(TMDSn[2]));
  //OBUFDS OBUFDS_green(.I(TMDS_shift_green[0]), .O(TMDSp[1]), .OB(TMDSn[1]));
  //OBUFDS OBUFDS_blue (.I(TMDS_shift_blue [0]), .O(TMDSp[0]), .OB(TMDSn[0]));
  //OBUFDS OBUFDS_clock(.I(pixclk), .O(TMDSp_clock), .OB(TMDSn_clock));
  assign o_red= TMDS_shift_red[0];
  assign o_grn= TMDS_shift_green[0];
  assign o_blu= TMDS_shift_blue[0];
`endif

endmodule
