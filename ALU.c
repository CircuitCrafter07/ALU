#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// Function prototypes for arithmetic and logical operations
uint32_t add(uint32_t A, uint32_t B, bool Cin, bool* Cout, bool* OVF);
uint32_t sub(uint32_t A, uint32_t B, bool Cin, bool* Cout, bool* OVF);
uint32_t and_op(uint32_t A, uint32_t B);
uint32_t or_op(uint32_t A, uint32_t B);
uint32_t xor_op(uint32_t A, uint32_t B);
uint32_t not_op(uint32_t A);
uint32_t sll(uint32_t A, uint32_t B);
uint32_t srl(uint32_t A, uint32_t B);
bool slt(uint32_t A, uint32_t B);
uint32_t mul(uint32_t A, uint32_t B);
uint32_t div_op(uint32_t A, uint32_t B);
uint32_t inc(uint32_t A);
uint32_t dec(uint32_t A);
bool equal(uint32_t A, uint32_t B);

// Main ALU function
uint32_t ALU(uint32_t A, uint32_t B, uint8_t OP, bool Cin, bool* Cout, bool* OVF, bool* Z) {
    uint32_t Result = 0;

    switch(OP) {
        case 0x00:  // ADD
            Result = add(A, B, Cin, Cout, OVF);
            break;
        case 0x01:  // SUB
            Result = sub(A, B, Cin, Cout, OVF);
            break;
        case 0x02:  // AND
            Result = and_op(A, B);
            break;
        case 0x03:  // OR
            Result = or_op(A, B);
            break;
        case 0x04:  // XOR
            Result = xor_op(A, B);
            break;
        case 0x05:  // SLL
            Result = sll(A, B);
            break;
        case 0x06:  // SRL
            Result = srl(A, B);
            break;
        case 0x07:  // SLT
            Result = slt(A, B) ? 1 : 0;
            break;
        case 0x08:  // MUL
            Result = mul(A, B);
            break;
        case 0x09:  // DIV
            Result = div_op(A, B);
            break;
        case 0x0A:  // INC
            Result = inc(A);
            break;
        case 0x0B:  // DEC
            Result = dec(A);
            break;
        case 0x0C:  // NOT
            Result = not_op(A);
            break;
        case 0x0D:  // EQUAL
            Result = equal(A, B) ? 1 : 0;
            break;
        default:
            Result = 0;
            break;
    }

    // Set Zero flag
    *Z = (Result == 0);

    return Result;
}

// Arithmetic and logic operation functions

uint32_t add(uint32_t A, uint32_t B, bool Cin, bool* Cout, bool* OVF) {
    uint64_t sum = (uint64_t)A + (uint64_t)B + (uint64_t)Cin;
    *Cout = (sum >> 32) & 1;  // Extract carry-out
    uint32_t Result = (uint32_t)sum;

    // Overflow detection
    *OVF = (((A & 0x80000000) == (B & 0x80000000)) && ((Result & 0x80000000) != (A & 0x80000000)));
    return Result;
}

uint32_t sub(uint32_t A, uint32_t B, bool Cin, bool* Cout, bool* OVF) {
    return add(A, ~B, Cin, Cout, OVF);  // Subtraction by adding two's complement
}

uint32_t and_op(uint32_t A, uint32_t B) {
    return A & B;
}

uint32_t or_op(uint32_t A, uint32_t B) {
    return A | B;
}

uint32_t xor_op(uint32_t A, uint32_t B) {
    return A ^ B;
}

uint32_t not_op(uint32_t A) {
    return ~A;
}

uint32_t sll(uint32_t A, uint32_t B) {
    return A << (B & 0x1F);  // Shift left, masking B to get lower 5 bits
}

uint32_t srl(uint32_t A, uint32_t B) {
    return A >> (B & 0x1F);  // Shift right, masking B to get lower 5 bits
}

bool slt(uint32_t A, uint32_t B) {
    return ((int32_t)A < (int32_t)B);
}

uint32_t mul(uint32_t A, uint32_t B) {
    return A * B;
}

uint32_t div_op(uint32_t A, uint32_t B) {
    if (B == 0) return 0xFFFFFFFF;  // Handle divide-by-zero
    return A / B;
}

uint32_t inc(uint32_t A) {
    return A + 1;
}

uint32_t dec(uint32_t A) {
    return A - 1;
}

bool equal(uint32_t A, uint32_t B) {
    return A == B;
}

// Example usage
int main() {
    uint32_t A = 15, B = 5;
    bool Cout, OVF, Z;
    uint32_t result;

    result = ALU(A, B, 0x00, 0, &Cout, &OVF, &Z);  // Perform ADD
    printf("Result (ADD): %u, Cout: %d, OVF: %d, Zero: %d\n", result, Cout, OVF, Z);

    result = ALU(A, B, 0x01, 0, &Cout, &OVF, &Z);  // Perform SUB
    printf("Result (SUB): %u, Cout: %d, OVF: %d, Zero: %d\n", result, Cout, OVF, Z);

    result = ALU(A, B, 0x02, 0, &Cout, &OVF, &Z);  // Perform AND
    printf("Result (AND): %u, Zero: %d\n", result, Z);

    return 0;
}
