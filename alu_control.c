#include <stdio.h>
#include <stdint.h>   // For uint32_t type

// ALU function prototype
void ALU32(uint32_t A, uint32_t B, uint8_t OP, uint8_t Cin, 
           uint32_t* Result, uint8_t* Cout, uint8_t* OVF, uint8_t* Z);

// Control function prototype
void ALUControl(uint32_t A, uint32_t B, uint8_t instruction, uint8_t Cin);

int main() {
    uint32_t A, B;
    uint8_t instruction, Cin;

    // Example inputs for testing
    A = 10;
    B = 5;
    instruction = 5;  // ADD operation
    Cin = 0;

    // Call the ALU control
    ALUControl(A, B, instruction, Cin);

    // Change instruction to perform other operations
    instruction = 6;  // SUB operation
    ALUControl(A, B, instruction, Cin);

    return 0;
}

// ALU function: performs the arithmetic or logic operation based on OP code
void ALU32(uint32_t A, uint32_t B, uint8_t OP, uint8_t Cin, 
           uint32_t* Result, uint8_t* Cout, uint8_t* OVF, uint8_t* Z) {
    
    uint64_t temp_result;  // Use 64-bit variable for intermediate result in case of overflow

    switch(OP) {
        case 0: // ADD
            temp_result = (uint64_t)A + (uint64_t)B + (uint64_t)Cin;
            *Result = (uint32_t)temp_result;
            *Cout = (temp_result > 0xFFFFFFFF);  // Set carry-out if overflow in addition
            break;

        case 1: // SUB
            temp_result = (uint64_t)A - (uint64_t)B - (uint64_t)Cin;
            *Result = (uint32_t)temp_result;
            *Cout = (temp_result > 0xFFFFFFFF);  // Borrow detection for subtraction
            break;

        case 2: // AND
            *Result = A & B;
            break;

        case 3: // OR
            *Result = A | B;
            break;

        case 4: // XOR
            *Result = A ^ B;
            break;

        case 5: // SLL (Shift Left Logical)
            *Result = A << (B & 0x1F);  // Only consider lower 5 bits of B for shifting
            break;

        case 6: // SRL (Shift Right Logical)
            *Result = A >> (B & 0x1F);  // Only consider lower 5 bits of B for shifting
            break;

        case 7: // SLT (Set Less Than)
            *Result = (A < B) ? 1 : 0;
            break;

        case 8: // MUL (Multiplication)
            *Result = A * B;
            break;

        case 9: // DIV (Division)
            if (B != 0)
                *Result = A / B;
            else
                *Result = 0xFFFFFFFF;  // Return a default value for division by zero
            break;

        case 10: // INC (Increment)
            *Result = A + 1;
            break;

        case 11: // DEC (Decrement)
            *Result = A - 1;
            break;

        case 12: // NOT
            *Result = ~A;
            break;

        case 13: // EQUAL
            *Result = (A == B) ? 1 : 0;
            break;

        default: // Default case: NOP (No Operation)
            *Result = 0;
            break;
    }

    // Zero flag
    *Z = (*Result == 0);

    // Overflow flag for ADD/SUB
    if (OP == 0) {
        *OVF = ((A & 0x80000000) == (B & 0x80000000)) && ((*Result & 0x80000000) != (A & 0x80000000));
    } else if (OP == 1) {
        *OVF = ((A & 0x80000000) != (B & 0x80000000)) && ((*Result & 0x80000000) != (A & 0x80000000));
    } else {
        *OVF = 0;  // No overflow detection for other operations
    }
}

// Control function that simulates the control of the ALU
void ALUControl(uint32_t A, uint32_t B, uint8_t instruction, uint8_t Cin) {
    uint32_t Result;
    uint8_t Cout, OVF, Z;

    // Call the ALU
    ALU32(A, B, instruction, Cin, &Result, &Cout, &OVF, &Z);

    // Print the result
    printf("Instruction: %d\n", instruction);
    printf("A: %u, B: %u, Cin: %u\n", A, B, Cin);
    printf("Result: %u, Cout: %u, OVF: %u, Z: %u\n\n", Result, Cout, OVF, Z);
}
