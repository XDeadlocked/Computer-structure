#pragma once
#include<stdio.h>
#include<stdint.h>

void process_instruction();

void R_type(uint8_t rs, uint8_t rt, uint8_t rd, uint8_t shamt, uint8_t funct);  // R-type Instruction
void I_type(uint8_t opcode, uint8_t rs, uint8_t rt, int16_t imm);   // I-type Instruction
void J_type(uint8_t opcode, uint32_t addr); // J-type Instruction

// R-type
void add(uint8_t rs, uint8_t rt, uint8_t rd);   // Signed addition
void addu(uint8_t rs, uint8_t rt, uint8_t rd);  // Unsigned addition
void sub(uint8_t rs, uint8_t rt, uint8_t rd);   // Signed subtraction
void subu(uint8_t rs, uint8_t rt, uint8_t rd);  // Unsigned subtraction
void mult(uint8_t rs, uint8_t rt);            // Signed multiplication
void multu(uint8_t rs, uint8_t rt);          // Unsigned multiplication
void div(uint8_t rs, uint8_t rt);          // Signed division
void divu(uint8_t rs, uint8_t rt);      // Unsigned division
void mfhi(uint8_t rd);                  // Read HI register
void mflo(uint8_t rd);                  // Read LO register
void mthi(uint8_t rs);                  // Write to HI register
void mtlo(uint8_t rs);                  // Write to LO register
void And(uint8_t rs, uint8_t rt, uint8_t rd);   // AND
void Or(uint8_t rs, uint8_t rt, uint8_t rd);    // OR
void Xor(uint8_t rs, uint8_t rt, uint8_t rd);   // XOR
void Nor(uint8_t rs, uint8_t rt, uint8_t rd);   // NOR
void sll(uint8_t rt, uint8_t rd, uint8_t shamt); // Logical left shift
void sllv(uint8_t rs, uint8_t rt, uint8_t rd);  // Variable logical left shift
void srl(uint8_t rt, uint8_t rd, uint8_t shamt); // Logical right shift
void srlv(uint8_t rs, uint8_t rt, uint8_t rd);  // Variable logical right shift
void sra(uint8_t rt, uint8_t rd, uint8_t shamt); // Arithmetic right shift
void srav(uint8_t rs, uint8_t rt, uint8_t rd);  // Variable arithmetic right shift
void slt(uint8_t rs, uint8_t rt, uint8_t rd);   // Signed comparison less than
void sltu(uint8_t rs, uint8_t rt, uint8_t rd);  // Unsigned comparison less than

// I-type
void addi(uint8_t rs, uint8_t rt, int16_t imm);   // Signed immediate addition
void addiu(uint8_t rs, uint8_t rt, int16_t imm);  // Unsigned immediate addition
void slti(uint8_t rs, uint8_t rt, int16_t imm);   // Signed immediate comparison less than
void sltiu(uint8_t rs, uint8_t rt, int16_t imm);  // Unsigned immediate comparison less than
void andi(uint8_t rs, uint8_t rt, int16_t imm);   // AND immediate
void ori(uint8_t rs, uint8_t rt, int16_t imm);    // OR immediate
void xori(uint8_t rs, uint8_t rt, int16_t imm);   // XOR immediate
void lui(uint8_t rt, int16_t imm);                // Load upper immediate
void lb(uint8_t rs, uint8_t rt, int16_t imm);     // Signed byte load
void lh(uint8_t rs, uint8_t rt, int16_t imm);     // Signed halfword load
void lw(uint8_t rs, uint8_t rt, int16_t imm);     // Signed word load
void lbu(uint8_t rs, uint8_t rt, int16_t imm);    // Unsigned byte load
void lhu(uint8_t rs, uint8_t rt, int16_t imm);    // Unsigned halfword load
void sb(uint8_t rs, uint8_t rt, int16_t imm);     // Byte store
void sh(uint8_t rs, uint8_t rt, int16_t imm);     // Halfword store
void sw(uint8_t rs, uint8_t rt, int16_t imm);     // Word store
void bltz(uint8_t rs, int16_t imm);               // Branch on less than zero
void bgez(uint8_t rs, int16_t imm);               // Branch on greater than or equal to zero
void bltzal(uint8_t rs, int16_t imm);             // Branch on less than zero and link
void bgezal(uint8_t rs, int16_t imm);             // Branch on greater than or equal to zero and link
void beq(uint8_t rs, uint8_t rt, int16_t imm);    // Branch on equal
void bne(uint8_t rs, uint8_t rt, int16_t imm);    // Branch on not equal
void blez(uint8_t rs, int16_t imm);               // Branch on less than or equal to zero
void bgtz(uint8_t rs, int16_t imm);               // Branch on greater than zero

// J-type
void j(uint32_t addr);      // Jump
void jal(uint32_t addr);    // Jump and link
void jr(uint8_t rs);        // Jump register
void jalr(uint8_t rs, uint8_t rd);  // Jump register and link

void syscall();             // System call
