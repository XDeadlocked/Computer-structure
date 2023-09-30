#include <stdio.h>
#include "shell.h"
#include "sim.h"
void process_instruction()
{
    uint32_t instr = mem_read_32(CURRENT_STATE.PC); // Read instruction from memory
    uint8_t opcode = instr >> 26;                   // Extract opcode (I-type, R-type, J-type)
    uint8_t rs = (instr >> 21) & 0x1f;              // Extract rs (I-type, R-type)
    uint8_t rt = (instr >> 16) & 0x1f;              // Extract rt (I-type, R-type)
    uint8_t rd = (instr >> 11) & 0x1f;              // Extract rd (R-type)
    uint8_t shamt = (instr >> 6) & 0x1f;            // Extract shamt (R-type)
    uint8_t funct = instr & 0x3f;                   // Extract funct (R-type)
    uint16_t imm = instr & 0xffff;                  // Extract imm (I-type)
    uint32_t addr = instr & 0x3ffffff;              // Extract addr (J-type)

    if (instr == 0) {}
    else if (opcode == 0)
    {
        R_type(rs, rt, rd, shamt, funct);
    }
    else if (opcode == 2)
    {
        J_type(opcode, addr);
    }
    else if (opcode == 3)
    {
        J_type(opcode, addr);
    }
    else
    {
        I_type(opcode, rs, rt, imm);
    }
    NEXT_STATE.PC += 4; // Increment PC by 4
}

// 0xC syscall

void R_type(uint8_t rs, uint8_t rt, uint8_t rd, uint8_t shamt, uint8_t funct)
{
    // Includes all R-type instructions
    if (funct == 0xc)
    {
        syscall();
        return;
    }
    switch (funct)
    {
    // 23 instructions
    case 0x20: add(rs, rt, rd); break;
    case 0x21: addu(rs, rt, rd); break;
    case 0x22: sub(rs, rt, rd); break;
    case 0x23: subu(rs, rt, rd); break;
    case 0x18: mult(rs, rt); break;
    case 0x19: multu(rs, rt); break;
    case 0x1a: div(rs, rt); break;
    case 0x1b: divu(rs, rt); break;
    case 0x10: mfhi(rd); break;
    case 0x12: mflo(rd); break;
    case 0x11: mthi(rs); break;
    case 0x13: mtlo(rs); break;
    case 0x24: And(rs, rt, rd); break;
    case 0x25: Or(rs, rt, rd); break;
    case 0x26: Xor(rs, rt, rd); break;
    case 0x27: Nor(rs, rt, rd); break;
    case 0x00: sll(rt, rd, shamt); break;
    case 0x04: sllv(rs, rt, rd); break;
    case 0x02: srl(rt, rd, shamt); break;
    case 0x06: srlv(rs, rt, rd); break;
    case 0x03: sra(rt, rd, shamt); break;
    case 0x07: srav(rs, rt, rd); break;
    case 0x2a: slt(rs, rt, rd); break;
    case 0x2b: sltu(rs, rt, rd); break;
    case 0x8: jr(rs); break;
    case 0x9: jalr(rs, rt); break;
    default: break;
    }
}

void I_type(uint8_t opcode, uint8_t rs, uint8_t rt, int16_t imm)
{
    // Includes all I-type instructions
    switch (opcode)
    {
    // 18 instructions
    case 0x08: addi(rs, rt, imm); break;
    case 0x09: addiu(rs, rt, imm); break;
    case 0x0a: slti(rs, rt, imm); break;
    case 0x0b: sltiu(rs, rt, imm); break;
    case 0x0c: andi(rs, rt, imm); break;
    case 0x0d: ori(rs, rt, imm); break;
    case 0x0e: xori(rs, rt, imm); break;
    case 0x0f: lui(rt, imm); break;
    case 0x20: lb(rs, rt, imm); break;
    case 0x21: lh(rs, rt, imm); break;
    case 0x23: lw(rs, rt, imm); break;
    case 0x24: lbu(rs, rt, imm); break;
    case 0x25: lhu(rs, rt, imm); break;
    case 0x28: sb(rs, rt, imm); break;
    case 0x29: sh(rs, rt, imm); break;
    case 0x2b: sw(rs, rt, imm); break;
    case 0x04: beq(rs, rt, imm); break;
    case 0x05: bne(rs, rt, imm); break;
    case 0x06: blez(rs, imm); break;
    case 0x07: bgtz(rs, imm); break;
    case 0x01:
    {
        switch (rt)
        {
        case 0x00: bltz(rs, imm); break;
        case 0x01: bgez(rs, imm); break;
        case 0x10: bltzal(rs, imm); break;
        case 0x11: bgezal(rs, imm); break;
        }
    }
    break;
    }
}

void J_type(uint8_t opcode, uint32_t addr)
{
    // Includes all J-type instructions
    switch (opcode)
    {
    // 2 instructions
    case 0x02: j(addr); break;
    case 0x03: jal(addr); break;
    }
}

// R-type
//1
void add(uint8_t rs, uint8_t rt, uint8_t rd)
{
    if(rd != 0)
    {
        NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] + CURRENT_STATE.REGS[rt];
    }
}
//2
void addu(uint8_t rs, uint8_t rt, uint8_t rd)
{
    if(rd != 0) // avoid write $zero
    {
        NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] + CURRENT_STATE.REGS[rt];
    }
}
//3
void sub(uint8_t rs, uint8_t rt, uint8_t rd)
{
    if(rd != 0)
    {
        NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] - CURRENT_STATE.REGS[rt];
    }
}
//4
void subu(uint8_t rs, uint8_t rt, uint8_t rd)
{
    if(rd != 0)
    {
        NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] - CURRENT_STATE.REGS[rt];
    }
}
// 5
void mult(uint8_t rs, uint8_t rt)
{
    /*
    The contents of general registers rs and rt are treated as 32-bit two's complement values for multiplication.
    There will be no integer overflow exception in any case.
    In 64-bit mode, the operands must be valid 32-bit sign-extended values.
    When the operation is complete, the low word of the double-precision result is loaded into special register LO,
    and the high word of the double-precision result is loaded into special register HI.
    */

    int64_t temp = (int64_t)CURRENT_STATE.REGS[rs] * (int64_t)CURRENT_STATE.REGS[rt];
    NEXT_STATE.LO = temp & 0xffffffff;
    NEXT_STATE.HI = (temp >> 32) & 0xffffffff;
}
// 6
void multu(uint8_t rs, uint8_t rt)
{
    /*
    The contents of general registers rs and rt are treated as unsigned values for multiplication.
    There will be no overflow exception in any case.
    In 64-bit mode, the operands must be valid 32-bit sign-extended values.
    When the operation is complete, the low word of the double-precision result is loaded into special register LO,
    and the high word of the double-precision result is loaded into special register HI.
    */

    uint64_t temp = (uint64_t)CURRENT_STATE.REGS[rs] * (uint64_t)CURRENT_STATE.REGS[rt];
    NEXT_STATE.LO = temp & 0xffffffff;
    NEXT_STATE.HI = (temp >> 32) & 0xffffffff;
}
// 7
void div(uint8_t rs, uint8_t rt)
{
    /*
    In 64-bit mode, operands must be valid sign-extended 32-bit values.
    Typically, this instruction is followed by other instructions for checking
    for division by zero and overflow. When the operation is complete, the
    quotient word of the doubleword result is loaded into special register LO,
    and the remainder word is loaded into special register HI.
    */
    if (CURRENT_STATE.REGS[rt] != 0)
    {
        NEXT_STATE.LO = (int32_t)CURRENT_STATE.REGS[rs] / (int32_t)CURRENT_STATE.REGS[rt];
        NEXT_STATE.HI = (int32_t)CURRENT_STATE.REGS[rs] % (int32_t)CURRENT_STATE.REGS[rt];
    }
}

// 8
void divu(uint8_t rs, uint8_t rt)
{
    /*
    When the operation is complete, the quotient word of the doubleword result
    is loaded into special register LO, and the remainder word is loaded into
    special register HI.
    */
    if (CURRENT_STATE.REGS[rt] != 0)
    {
        NEXT_STATE.LO = (uint32_t)CURRENT_STATE.REGS[rs] / (uint32_t)CURRENT_STATE.REGS[rt];
        NEXT_STATE.HI = (uint32_t)CURRENT_STATE.REGS[rs] % (uint32_t)CURRENT_STATE.REGS[rt];
    }
}

// 9
void mfhi(uint8_t rd)
{
    /*
    Read the high-order word of the doubleword result from special register HI
    and load it into general register rd.
    */
    if (rd != 0)
    {
        NEXT_STATE.REGS[rd] = CURRENT_STATE.HI;
    }
}

// 10
void mflo(uint8_t rd)
{
    /*
    Read the low-order word of the doubleword result from special register LO
    and load it into general register rd.
    */
    if (rd != 0)
    {
        NEXT_STATE.REGS[rd] = CURRENT_STATE.LO;
    }
}

// 11
void mthi(uint8_t rs)
{
    /*
    Load the contents of general register rs into special register HI.
    */
    NEXT_STATE.HI = CURRENT_STATE.REGS[rs];
}

// 12
void mtlo(uint8_t rs)
{
    /*
    Load the contents of general register rs into special register LO.
    */
    NEXT_STATE.LO = CURRENT_STATE.REGS[rs];
}

// 13
void And(uint8_t rs, uint8_t rt, uint8_t rd)
{
    /*
    Treat the contents of general registers rs and rt as 32-bit binary
    two's-complement values and perform a bitwise logical AND operation on
    them. Store the result in general register rd.
    */
    if (rd != 0)
    {
        NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] & CURRENT_STATE.REGS[rt];
    }
}

// 14
void Or(uint8_t rs, uint8_t rt, uint8_t rd)
{
    /*
    Treat the contents of general registers rs and rt as 32-bit binary
    two's-complement values and perform a bitwise logical OR operation on
    them. Store the result in general register rd.
    */
    if (rd != 0)
    {
        NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] | CURRENT_STATE.REGS[rt];
    }
}

// 15
void Xor(uint8_t rs, uint8_t rt, uint8_t rd)
{
    /*
    Treat the contents of general registers rs and rt as 32-bit binary
    two's-complement values and perform a bitwise logical XOR operation on
    them. Store the result in general register rd.
    */
    if (rd != 0)
    {
        NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] ^ CURRENT_STATE.REGS[rt];
    }
}

// 16
void Nor(uint8_t rs, uint8_t rt, uint8_t rd)
{
    /*
    Treat the contents of general registers rs and rt as 32-bit binary
    two's-complement values, perform a bitwise logical OR operation on them,
    and then negate the result. Store the result in general register rd.
    */
    if (rd != 0)
    {
        NEXT_STATE.REGS[rd] = ~(CURRENT_STATE.REGS[rs] | CURRENT_STATE.REGS[rt]);
    }
}

// 17
void sll(uint8_t rt, uint8_t rd, uint8_t shamt)
{
    /*
    Treat the contents of general register rt as a 32-bit binary two's-complement
    value and perform a logical left shift operation by shamt bits. Store the result
    in general register rd.
    */
    if (rd != 0)
    {
        NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rt] << shamt;
    }
}

// 18
void sllv(uint8_t rs, uint8_t rt, uint8_t rd)
{
    /*
    Treat the contents of general register rt as a 32-bit binary two's-complement
    value and perform a logical left shift operation by the least significant 5 bits
    of the value in general register rs. Store the result in general register rd.
    */
    if (rd != 0)
    {
        NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rt] << (CURRENT_STATE.REGS[rs] & 0x1F);
    }
}

// 19
void srl(uint8_t rt, uint8_t rd, uint8_t shamt)
{
    /*
    Treat the contents of general register rt as a 32-bit binary two's-complement
    value and perform a logical right shift operation by shamt bits. Store the result
    in general register rd.
    */
    if (rd != 0)
    {
        NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rt] >> shamt;
    }
}

// 20
void srlv(uint8_t rs, uint8_t rt, uint8_t rd)
{
    /*
    Treat the contents of general register rt as a 32-bit binary two's-complement
    value and perform a logical right shift operation by the least significant 5 bits
    of the value in general register rs. Store the result in general register rd.
    */
    if (rd != 0)
    {
        NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rt] >> (CURRENT_STATE.REGS[rs] & 0x1F);
    }
}

// 21
void sra(uint8_t rt, uint8_t rd, uint8_t shamt)
{
    /*
    Treat the contents of general register rt as a 32-bit binary two's-complement
    value and perform an arithmetic right shift operation by shamt bits. Store the
    result in general register rd.
    */
    if (rd != 0)
    {
        NEXT_STATE.REGS[rd] = (int32_t)CURRENT_STATE.REGS[rt] >> shamt;
    }
}

// 22
void srav(uint8_t rs, uint8_t rt, uint8_t rd)
{
    /*
    Treat the contents of general register rt as a 32-bit binary two's-complement
    value and perform an arithmetic right shift operation by the least significant 5 bits
    of the value in general register rs. Store the result in general register rd.
    */
    if (rd != 0)
    {
        NEXT_STATE.REGS[rd] = (int32_t)CURRENT_STATE.REGS[rt] >> (CURRENT_STATE.REGS[rs] & 0x1F);
    }
}

// 23
void slt(uint8_t rs, uint8_t rt, uint8_t rd)
{
    /*
    Treat the contents of general registers rs and rt as 32-bit binary two's-complement
    values and perform a signed comparison. If rs < rt, set general register rd to 1,
    otherwise set it to 0.
    */
    if (rd != 0)
    {
        if ((int32_t)CURRENT_STATE.REGS[rs] < (int32_t)CURRENT_STATE.REGS[rt])
        {
            NEXT_STATE.REGS[rd] = 1;
        }
        else
        {
            NEXT_STATE.REGS[rd] = 0;
        }
    }
}

// 24
void sltu(uint8_t rs, uint8_t rt, uint8_t rd)
{
    /*
    Treat the contents of general registers rs and rt as 32-bit binary two's-complement
    values and perform an unsigned comparison. If rs < rt, set general register rd to 1,
    otherwise set it to 0.
    */
    if (rd != 0)
    {
        if (CURRENT_STATE.REGS[rs] < CURRENT_STATE.REGS[rt])
        {
            NEXT_STATE.REGS[rd] = 1;
        }
        else
        {
            NEXT_STATE.REGS[rd] = 0;
        }
    }
}

// I-type
// 25
void addi(uint8_t rs, uint8_t rt, int16_t imm)
{
    /*
    Treat the contents of general register rs as a 32-bit binary two's-complement value,
    sign-extend the 16-bit immediate value to 32 bits, and add them together. Store the
    result in general register rt.
    */
    if (rt != 0)
    {
        NEXT_STATE.REGS[rt] = (int32_t)CURRENT_STATE.REGS[rs] + imm;
    }
}

// 26
void addiu(uint8_t rs, uint8_t rt, int16_t imm)
{
    /*
    Treat the contents of general register rs as a 32-bit binary two's-complement value,
    sign-extend the 16-bit immediate value to 32 bits, and add them together. Store the
    result in general register rt.
    */
    if (rt != 0)
    {
        NEXT_STATE.REGS[rt] = (int32_t)CURRENT_STATE.REGS[rs] + imm;
    }
}

// 27
void slti(uint8_t rs, uint8_t rt, int16_t imm)
{
    /*
    Treat the contents of general register rs as a 32-bit binary two's-complement value,
    sign-extend the 16-bit immediate value to 32 bits, and perform a signed comparison.
    If rs < imm, set general register rt to 1, otherwise set it to 0.
    */
    if (rt != 0)
    {
        if ((int32_t)CURRENT_STATE.REGS[rs] < (int32_t)imm)
        {
            NEXT_STATE.REGS[rt] = 1;
        }
        else
        {
            NEXT_STATE.REGS[rt] = 0;
        }
    }
}

// 28
void sltiu(uint8_t rs, uint8_t rt, int16_t imm)
{
    /*
    Treat the contents of general register rs as a 32-bit binary two's-complement value,
    sign-extend the 16-bit immediate value to 32 bits, and perform an unsigned comparison.
    If rs < imm, set general register rt to 1, otherwise set it to 0.
    */
    if (rt != 0)
    {
        if (CURRENT_STATE.REGS[rs] < imm)
        {
            NEXT_STATE.REGS[rt] = 1;
        }
        else
        {
            NEXT_STATE.REGS[rt] = 0;
        }
    }
}

// 29
void andi(uint8_t rs, uint8_t rt, int16_t imm)
{
    /*
    Treat the contents of general register rs as a 32-bit binary two's-complement value
    and perform a bitwise logical AND operation with the zero-extended 16-bit immediate
    value. Store the result in general register rt.
    */
    if (rt != 0)
    {
        NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] & (uint32_t)imm;
    }
}

// 30
void ori(uint8_t rs, uint8_t rt, int16_t imm)
{
    if (rt != 0)
    {
        // Zero-extend the 16-bit immediate value and perform a logical OR operation with
        // the contents of general register rs. Store the result in general register rt.
        int32_t result = (int32_t)CURRENT_STATE.REGS[rs] | (int32_t)(imm & 0xFFFF);
        NEXT_STATE.REGS[rt] = result;
    }
}

// 31
void xori(uint8_t rs, uint8_t rt, int16_t imm)
{
    /*
    Treat the contents of general register rs as a 32-bit binary two's-complement value
    and perform a bitwise logical XOR operation with the zero-extended 16-bit immediate
    value. Store the result in general register rt.
    */
    if (rt != 0)
    {
        NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] ^ (uint32_t)imm;
    }
}

// 32
void lui(uint8_t rt, int16_t imm)
{
    /*
    Treat the sign-extended 16-bit immediate value as a 32-bit binary two's-complement value
    and left-shift it by 16 bits. Store the result in general register rt.
    */
    if (rt != 0)
    {
        NEXT_STATE.REGS[rt] = imm << 16;
    }
}
// 33
void lb(uint8_t rs, uint8_t rt, int16_t imm)
{
    if (rt != 0)
    {
        // Sign-extend the 16-bit offset and add it to the contents of general register rs
        // to form a virtual address.
        int32_t addr = (int32_t)CURRENT_STATE.REGS[rs] + (int32_t)imm;

        // Read the byte content from memory at the specified effective address, sign-extend
        // it, and load it into general register rt.
        int32_t data = (int32_t)mem_read_32(addr);
        int32_t sign_extended_data = (data << 24) >> 24; // Sign-extend to 32 bits
        NEXT_STATE.REGS[rt] = sign_extended_data;
    }
}
// 34
void lh(uint8_t rs, uint8_t rt, int16_t imm)
{
	if(rt != 0)
	{
	// Sign-extend the 16-bit offset and add it to the content of register rs to form a virtual address
        int32_t addr = (int32_t)CURRENT_STATE.REGS[rs] + (int32_t)imm;

        // Check if the least significant bit of the effective address is non-zero, if so, trigger an address error exception
        if (addr & 0x1)
        {
        }
        else
        {
        // Read the half-word content at the specified effective address from memory, sign-extend it, and then load it into register rt
            int32_t data = (int32_t)mem_read_32(addr);
            int32_t sign_extended_data = (data << 16) >> 16; // Sign-extend to 32 bits
            NEXT_STATE.REGS[rt] = sign_extended_data;
        }
	}
}
// 35
void lw(uint8_t rs, uint8_t rt, int16_t imm)
{
    /*
    Treat the content of general register rs as a valid 32-bit address,
    and add it to the sign-extended 16-bit immediate as a 32-bit two's complement value,
    then read a word from memory in units of words. Store the result in general register rt.
    */
    if(rt != 0)
    {
        uint32_t addr = (int32_t) CURRENT_STATE.REGS[rs] + imm;
	NEXT_STATE.REGS[rt] = mem_read_32(addr);
    }
}
// 36
void lbu(uint8_t rs, uint8_t rt, int16_t imm)
{
    /*
    Treat the content of general register rs as a valid 32-bit address,
    and add it to the sign-extended 16-bit immediate as a 32-bit two's complement value,
    then read a byte from memory. Store the lowest 8 bits in general register rt,
    and set the remaining bits to 0.
    */
    if(rt != 0)
    {
	uint32_t addr = (int32_t) CURRENT_STATE.REGS[rs] + imm;
	NEXT_STATE.REGS[rt] = mem_read_32(addr) & 0xff;
    }
}
// 37
void lhu(uint8_t rs, uint8_t rt, int16_t imm)
{
    /*
    Treat the content of general register rs as a valid 32-bit address,
    and add it to the sign-extended 16-bit immediate as a 32-bit two's complement value,
    then read a half-word from memory. Store the lowest 16 bits in general register rt,
    and set the remaining bits to 0.
    */
    if(rt != 0)
    {
        uint32_t addr = (int32_t) CURRENT_STATE.REGS[rs] + imm;
	    NEXT_STATE.REGS[rt] = mem_read_32(addr) & 0xffff;
    }
}
// 38
void sb(uint8_t rs, uint8_t rt, int16_t imm)
{
    /*
    Treat the content of general register rt as a valid 32-bit address,
    and add it to the sign-extended 16-bit immediate as a 32-bit two's complement value,
    then store the lowest 8 bits of general register rs in memory.
    */
    uint32_t addr = (int32_t) CURRENT_STATE.REGS[rs] + imm;
    uint32_t data = mem_read_32(addr) & ~0xff;
    data = data | (CURRENT_STATE.REGS[rt] & 0xff);
    mem_write_32(addr, data);    
}
// 39
void sh(uint8_t rs, uint8_t rt, int16_t imm)
{
    /*
    Treat the content of general register rt as a valid 32-bit address,
    and add it to the sign-extended 16-bit immediate as a 32-bit two's complement value,
    then store the lowest 16 bits of general register rs in memory.
    */
    uint32_t addr = (int32_t) CURRENT_STATE.REGS[rs] + imm;
    uint32_t data = mem_read_32(addr) & ~0xffff;
    data = data | (CURRENT_STATE.REGS[rt] & 0xffff);
    mem_write_32(addr, data);
}
// 40
void sw(uint8_t rs, uint8_t rt, int16_t imm)
{
    /*
    Treat the content of general register rt as a valid 32-bit address,
    and add it to the sign-extended 16-bit immediate as a 32-bit two's complement value,
    then store the content of general register rs in memory.
    */
    uint32_t addr = (int32_t) CURRENT_STATE.REGS[rs] + imm;
    mem_write_32(addr, CURRENT_STATE.REGS[rt]);
}
// 41
void beq(uint8_t rs, uint8_t rt, int16_t imm)
{
    /*
    Treat the content of general register rs and general register rt as 32-bit two's complement values,
    and compare them with a signed comparison. If rs = rt, set PC to PC + 4 + (imm << 2),
    otherwise set PC to PC + 4.
    */
    if(CURRENT_STATE.REGS[rs] == CURRENT_STATE.REGS[rt])
    {
        NEXT_STATE.PC = CURRENT_STATE.PC + (imm<<2);
    }	
}
// 42
void bne(uint8_t rs, uint8_t rt, int16_t imm)
{
    /*
    Treat the content of general register rs and general register rt as 32-bit two's complement values,
    and compare them with a signed comparison. If rs != rt, set PC to PC + 4 + (imm << 2),
    otherwise set PC to PC + 4.
    */
    if(CURRENT_STATE.REGS[rs] != CURRENT_STATE.REGS[rt])
    {
        NEXT_STATE.PC = CURRENT_STATE.PC + (imm << 2);
    }
}
// 43
void blez(uint8_t rs, int16_t imm)
{
    /*
    Treat the content of general register rs as a 32-bit two's complement value,
    and compare it with a signed comparison. If rs <= 0, set PC to PC + 4 + (imm << 2),
    otherwise set PC to PC + 4.
    */
    if((int32_t)CURRENT_STATE.REGS[rs] <= 0)
    {
        NEXT_STATE.PC = CURRENT_STATE.PC + (imm << 2);
    }

}
// 44
void bltz(uint8_t rs, int16_t imm)
{
    /*
    The branch target address is calculated based on the address of the instruction in the delay slot and a 16-bit offset,
    then left-shifted by two and sign-extended.
    If the sign bit of the content of general register rs is set to 1,
    the program will jump to the target address and execute one instruction in the delay slot.
    */
    if((int32_t)CURRENT_STATE.REGS[rs] < 0)
    {
        NEXT_STATE.PC = CURRENT_STATE.PC + (imm << 2);
    }
}
// 45
void bgez(uint8_t rs, int16_t imm)
{
    /*
    Treat the content of general register rs as a 32-bit two's complement value,
    and compare it with a signed comparison. If rs >= 0, set PC to PC + 4 + (imm << 2),
    otherwise set PC to PC + 4.
    */
    if((int32_t)CURRENT_STATE.REGS[rs] >= 0)
    {
        NEXT_STATE.PC = CURRENT_STATE.PC + (imm << 2);
    }
}

void bgtz(uint8_t rs, int16_t imm) 
{
	if((int32_t) CURRENT_STATE.REGS[rs] > 0)
    {
        NEXT_STATE.PC = CURRENT_STATE.PC + (imm<<2);
    }
}


// 46
void bltzal(uint8_t rs, int16_t imm)
{
    /*
    Treat the content of general register rs as a 32-bit two's complement value,
    and compare it with a signed comparison. If rs < 0, set PC to PC + 4 + (imm << 2),
    otherwise set PC to PC + 4. Store the value of PC + 8 in general register 31.
    */
    if((int32_t) CURRENT_STATE.REGS[rs] < 0)
    {
        NEXT_STATE.PC = CURRENT_STATE.PC + (imm << 2);
        NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 4;
    }

}
// 47
void bgezal(uint8_t rs, int16_t imm)
{
    /*
    Treat the content of general register rs as a 32-bit two's complement value,
    and compare it with a signed comparison. If rs >= 0, set PC to PC + 4 + (imm << 2),
    otherwise set PC to PC + 4. Store the value of PC + 8 in general register 31.
    */
    if((int32_t)CURRENT_STATE.REGS[rs] >= 0)
    {
        NEXT_STATE.PC = CURRENT_STATE.PC + (imm << 2);
        NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 4;
    }
}


// J-Type
// 48
void j(uint32_t addr)
{
    /*
    Set the high 4 bits of PC to the high 4 bits of PC + 4,
    and set the low 28 bits of PC to the low 28 bits of addr.
    */
    NEXT_STATE.PC = (addr << 2) - 4;
}
// 49
void jal(uint32_t addr)
{
    /*
    Set the high 4 bits of PC to the high 4 bits of PC + 4,
    and set the low 28 bits of PC to the low 28 bits of addr.
    Store the value of PC + 8 in general register 31.
    */
    NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 4;
    NEXT_STATE.PC = (addr << 2) - 4;
}
// 50
void jr(uint8_t rs)
{
    /*
    Treat the content of general register rs as a valid 32-bit address,
    and set it as the value of PC.
    */
    NEXT_STATE.PC = CURRENT_STATE.REGS[rs] - 4;
}
// 51
void jalr(uint8_t rs, uint8_t rd)
{
    uint8_t temp = rd == 0 ? 31 : rd;
    NEXT_STATE.REGS[temp] = CURRENT_STATE.PC + 4;
    NEXT_STATE.PC = CURRENT_STATE.REGS[rs] - 4;
}

// Syscall
// 52
void syscall() 
{
    /*When the SYSCALL instruction is executed, if the value of register $v0 (register 2) is 0x0A (decimal 10),
    then the go command should stop its simulation loop and return to the prompt of the simulator.
    If $v0 has any other value, the instruction should have no effect.*/ 
    if(CURRENT_STATE.REGS[2] == 0x0a)
    {
        RUN_BIT = 0;
    }
}


