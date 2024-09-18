module ALU_Control (
    input clk,                    // Clock input for synchronous operation
    input reset,                  // Reset input to initialize or reset the system
    input [31:0] A_in,            // 32-bit input A
    input [31:0] B_in,            // 32-bit input B
    input [3:0] instruction,      // 4-bit instruction to select ALU operation
    input Cin_in,                 // Carry-in for ALU
    output [31:0] Result_out,     // ALU result output
    output Cout_out,              // Carry-out output from ALU
    output OVF_out,               // Overflow flag output from ALU
    output Z_out                 // Zero flag output from ALU
);

    // Registers for inputs
    reg [31:0] A, B;              // ALU input registers
    reg [3:0] OP;                 // ALU operation code register
    reg Cin;                      // Carry-in register

    // Outputs from ALU
    wire [31:0] ALU_Result;
    wire ALU_Cout, ALU_OVF, ALU_Z;

    // ALU instantiation
    ALU_32 ALU (
        .A(A), 
        .B(B), 
        .OP(OP), 
        .Cin(Cin), 
        .Result(ALU_Result), 
        .Cout(ALU_Cout), 
        .OVF(ALU_OVF), 
        .Z(ALU_Z)
    );

    // Registering the outputs of the ALU
    assign Result_out = ALU_Result;
    assign Cout_out = ALU_Cout;
    assign OVF_out = ALU_OVF;
    assign Z_out = ALU_Z;

    // Control unit logic (state machine or simple logic)
    always @(posedge clk or posedge reset) begin
        if (reset) begin
            A <= 32'b0;
            B <= 32'b0;
            OP <= 4'b0000;        // Default operation: ADD
            Cin <= 1'b0;
        end
        else begin
            A <= A_in;            // Load input A
            B <= B_in;            // Load input B
            OP <= instruction;    // Select operation based on instruction input
            Cin <= Cin_in;        // Carry-in input for the ALU
        end
    end

endmodule

module ALU_32(
    input [31:0] A,    // 32-bit input A
    input [31:0] B,    // 32-bit input B
    input [3:0] OP,    // 4-bit operation code
    input Cin,         // Carry-in
    output reg [31:0] Result,  // 32-bit result
    output reg Cout,           // Carry-out
    output reg OVF,            // Overflow flag
    output reg Z               // Zero flag
);

always @(*) begin
    case(OP)
        4'b0000: {Cout, Result} = A + B + Cin; // ADD
        4'b0001: {Cout, Result} = A - B - Cin; // SUB
        4'b0010: Result = A & B;               // AND
        4'b0011: Result = A | B;               // OR
        4'b0100: Result = A ^ B;               // XOR
        4'b0101: Result = A << B;              // SLL
        4'b0110: Result = A >> B;              // SRL
        4'b0111: Result = (A < B) ? 32'b1 : 32'b0; // SLT
        4'b1000: Result = A * B;               // MUL
        4'b1001: Result = A / B;               // DIV
        4'b1010: Result = A + 1;               // INC
        4'b1011: Result = A - 1;               // DEC
        4'b1100: Result = ~A;                  // NOT
        4'b1101: Result = (A == B) ? 32'b1 : 32'b0; // EQUAL
        default: Result = 32'b0;               // Default: No Operation
    endcase

    // Zero flag
    Z = (Result == 32'b0) ? 1'b1 : 1'b0;

    // Overflow detection for addition and subtraction
    if ((OP == 4'b0000 || OP == 4'b0001) && 
        ((A[31] & B[31] & ~Result[31]) | (~A[31] & ~B[31] & Result[31])))
        OVF = 1'b1;
    else
        OVF = 1'b0;
end

endmodule
