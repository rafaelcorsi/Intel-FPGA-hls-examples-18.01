// This testbench sets alphas to constant and sweeps 5 values for x
`timescale 1 ps / 1 ps
module tb();
  logic         clk;
  logic         resetn;
  logic [31:0]  lpf_alpha, lpf_x, hpf_alpha, hpf_returndata;
  logic         hpf_return_valid, hpf_return_stall;
  logic         lpf_call_valid, lpf_call_stall;

  // generate clock
  initial begin
    clk = 1'b0;
    forever #10 clk = ~clk;
  end

  // generate reset (active low), hold for 1 clock cycle
  initial begin
    resetn = 1'b1;
    @(posedge clk);
    resetn = 1'b0;
    @(posedge clk);
    resetn = 1'b1;
  end

  // generate test inputs
  localparam NUM_TEST_INPUT = 5;
  logic [31:0] testin [0:NUM_TEST_INPUT-1] = '{ 32'h0, 32'h3f800000, 32'hbf800000, 32'h41200000, 32'hc1200000};    // inputs to drive x
  int c0;
  initial begin
    lpf_call_valid <= 1'b0;
    lpf_alpha <= 32'h3f800000;  // set alphas to 1.0f
    hpf_alpha <= 32'h3f800000;

    wait(resetn == 1'b0);
    wait(resetn == 1'b1);
    @(posedge clk);
    lpf_call_valid <= 1'b1;      // start sending data

    c0 = 0;
    lpf_x <= testin[c0];        // init x to first input
    while(c0 < NUM_TEST_INPUT) begin
      @(posedge clk);
      if(!lpf_call_stall) begin
        c0 = c0 + 1;
        lpf_x <= testin[c0];    // load next testin value to x
      end
    end

    @(posedge clk);
    lpf_call_valid <= 1'b0;      // stop driving valid data
  end

  // instantiate a top module and hook up connections
  top DUT(
    .clk_clk(clk),
    .reset_reset_n(resetn),
    .top_hpf_0_alpha_data(hpf_alpha),
    .top_hpf_0_return_valid(hpf_return_valid),
    .top_hpf_0_return_stall(hpf_return_stall),
    .top_hpf_0_returndata_data(hpf_returndata),
    .top_lpf_0_alpha_data(lpf_alpha),
    .top_lpf_0_call_stall(lpf_call_stall),
    .top_lpf_0_call_valid(lpf_call_valid),
    .top_lpf_0_x_data(lpf_x)
  );

  // checker
  logic [31:0] golden [0:NUM_TEST_INPUT-1] = '{ 32'h0, 32'h3f800000, 32'h3f800000, 32'h41400000, 32'h41600000};
  int count;
  bit pass;
  initial begin
    hpf_return_stall <= 1'b0;   // set no stall from downstream
    count = 0;
    pass = 1;
    while(count < NUM_TEST_INPUT) begin
      @(posedge clk);
      if(hpf_return_valid) begin
        if(hpf_returndata != golden[count]) begin
          pass = 0;
          $display("Failed. Expecting: %h, but output is: %h", golden[count], hpf_returndata);
        end
        count = count + 1;
      end
    end
    
    if(pass)
      $display("PASSED");
    else
      $error("FAILED");
    
    $finish;
  end

endmodule
