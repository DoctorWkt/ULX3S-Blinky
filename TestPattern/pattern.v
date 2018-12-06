`default_nettype none

module pattern(input i_tmdsclk, i_pixclk, output red, grn, blu, o_rd);

  wire [7:0] red, grn, blu;
  wire [23:0] pixel;
  assign red= pixel[23:16];
  assign grn= pixel[15:8];
  assign blu= pixel[7:0];

/* verilator lint_off UNUSED */
  wire o_red;
  wire o_grn;
  wire o_blu;
/* verilator lint_on UNUSED */
  wire o_rd, o_newline, o_newframe;


  llhdmi llhdmi_instance(
    .i_tmdsclk(i_tmdsclk), .i_pixclk(i_pixclk),
    .i_reset(1'b0), .i_red(red), .i_grn(grn), .i_blu(blu),
    .o_rd(o_rd), .o_newline(o_newline), .o_newframe(o_newframe),
    .o_red(o_red), .o_grn(o_grn), .o_blu(o_blu));

  vgatestsrc vgatestsrc_instance(
    .i_pixclk(i_pixclk), .i_reset(1'd0),
    .i_rd(o_rd), .i_newline(o_newline), .i_newframe(o_newframe),
    .o_pixel(pixel));

endmodule
