module synchronizer(
    input logic clock,
    input logic resetn,
    output logic resetn_out
);

reg [2:0] sync_resetn;

assign resetn_out = sync_resetn[2];

always @(posedge clock or negedge resetn) begin
    if (!resetn) begin
        sync_resetn <= 3'b0;
    end else begin
        sync_resetn <= {sync_resetn[1:0], 1'b1};
    end
end
endmodule
