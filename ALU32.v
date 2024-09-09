module ALU32(
    input [31:0] A,    // 32-bit input A
    input [31:0] B,    // 32-bit input B
    input [3:0] OP,    // 4-bit operation code
    input Cin,         // Carry-in for addition
    output [31:0] Result,  // 32-bit result
    output Cout,           // Carry-out
    output OVF,            // Overflow flag
    output Z               // Zero flag
);

// Internal wires for arithmetic and logic operations
wire [31:0] sum, difference, and_res, or_res, xor_res, not_res, sll_res, srl_res;
wire slt_res, equal_res;
wire carry_out_add, carry_out_sub;
wire ovf_add, ovf_sub;

// ADD operation (using basic gates to show the structure)
assign {carry_out_add, sum} = A + B + Cin;

// SUB operation (using two's complement of B)
assign {carry_out_sub, difference} = A + (~B + 1) + Cin;

// AND operation (bitwise AND)
assign and_res = A & B;

// OR operation (bitwise OR)
assign or_res = A | B;

// XOR operation (bitwise XOR)
assign xor_res = A ^ B;

// NOT operation (bitwise NOT)
assign not_res = ~A;

// Shift Left Logical (SLL)
assign sll_res = A << B[4:0];

// Shift Right Logical (SRL)
assign srl_res = A >> B[4:0];

// SLT operation (Set on Less Than, signed comparison)
assign slt_res = ($signed(A) < $signed(B)) ? 1'b1 : 1'b0;

// Equality check
assign equal_res = (A == B);

// Overflow detection for ADD and SUB operations
assign ovf_add = (A[31] == B[31]) && (sum[31] != A[31]);
assign ovf_sub = (A[31] != B[31]) && (difference[31] != A[31]);

// Output selection based on the operation code (OP)
assign Result = (OP == 4'b0000) ? sum :            // ADD
                (OP == 4'b0001) ? difference :     // SUB
                (OP == 4'b0010) ? and_res :        // AND
                (OP == 4'b0011) ? or_res :         // OR
                (OP == 4'b0100) ? xor_res :        // XOR
                (OP == 4'b0101) ? sll_res :        // SLL
                (OP == 4'b0110) ? srl_res :        // SRL
                (OP == 4'b0111) ? {31'b0, slt_res} : // SLT
                (OP == 4'b1100) ? not_res :        // NOT
                (OP == 4'b1101) ? {31'b0, equal_res} : // EQUAL
                32'b0;                             // Default: No Operation

// Carry-out is relevant for ADD and SUB
assign Cout = (OP == 4'b0000) ? carry_out_add :     // ADD
              (OP == 4'b0001) ? carry_out_sub :     // SUB
              1'b0;                                // Otherwise, no carry-out

// Overflow flag
assign OVF = (OP == 4'b0000) ? ovf_add :            // ADD
             (OP == 4'b0001) ? ovf_sub :            // SUB
             1'b0;                                  // Otherwise, no overflow

// Zero flag (Z = 1 if Result is zero)
assign Z = (Result == 32'b0) ? 1'b1 : 1'b0;

endmodule
