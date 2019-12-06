
#include <iostream>

#include <unistd.h>
#include <fcntl.h>

#include "riscv.h"

Riscv::Riscv(uint32_t memorySize):
    memory(memorySize)
{}

size_t Riscv::getMemorySize()
{
    return memory.getSize();
}

reg_t Riscv::getReg(int num)
{
    return hart.getReg(num);
}

void Riscv::setReg(int num, reg_t val)
{
    hart.setReg(num, val);
}

/*  Integer register-immediate instructions  */

#define INT_REG_IMM(name, op)                          \
void Riscv::name(const Instruction& instr)             \
{                                                      \
    setReg(instr.rd, getReg(instr.rs1) op instr.imm);  \
    hart.updatePc();                                   \
}

INT_REG_IMM(addi, +)
INT_REG_IMM(slti, <)
INT_REG_IMM(andi, &)
INT_REG_IMM(ori, |)
INT_REG_IMM(xori, ^)

#undef INT_REG_IMM

#define INT_REG_IMM(name, val)              \
void Riscv::name(const Instruction& instr)  \
{                                           \
    setReg(instr.rd, (val));                \
    hart.updatePc();                        \
}

INT_REG_IMM(sltiu, (uint32_t)(getReg(instr.rs1)) < (uint32_t)(instr.imm))

// shift instructions have shamt in rs2 field
INT_REG_IMM(slli, getReg(instr.rs1) << instr.rs2)
INT_REG_IMM(srli, (uint32_t)getReg(instr.rs1) >> instr.rs2)
INT_REG_IMM(srai, getReg(instr.rs1) >> instr.rs2)

INT_REG_IMM(lui, (instr.imm << 12) & ~0xFFF)
INT_REG_IMM(auipc, ((instr.imm << 12) & ~0xFFF) + hart.getPc())

#undef INT_REG_IMM

/*  Integer register-register instructions  */

#define INT_REG_REG(name, op)                                  \
void Riscv::name(const Instruction& instr)                     \
{                                                              \
    setReg(instr.rd, getReg(instr.rs1) op getReg(instr.rs2));  \
    hart.updatePc();                                           \
}

INT_REG_REG(add_, +)
INT_REG_REG(sub, -)
INT_REG_REG(slt, <)
INT_REG_REG(and_, &)
INT_REG_REG(or_, |)
INT_REG_REG(xor_, ^)

#undef INT_REG_REG

#define INT_REG_REG(name, val)              \
void Riscv::name(const Instruction& instr)  \
{                                           \
    setReg(instr.rd, (val));                \
    hart.updatePc();                        \
}

INT_REG_REG(sltu, (uint32_t)getReg(instr.rs1) < (uint32_t)getReg(instr.rs2))

INT_REG_REG(sll, getReg(instr.rs1) << (getReg(instr.rs2) & 0x1F))
INT_REG_REG(srl, (uint32_t)getReg(instr.rs1) >> (getReg(instr.rs2) & 0x1F))
INT_REG_REG(sra, getReg(instr.rs1) >> (getReg(instr.rs2) & 0x1F))

#undef INT_REG_REG

/*  Control transfer instructions  */

void Riscv::jal(const Instruction& instr)
{
    uint32_t new_pc = hart.getPc() + (instr.imm << 1);
    setReg(instr.rd, hart.getPc() + 4);
    hart.updatePc(new_pc);
}

void Riscv::jalr(const Instruction& instr)
{
    uint32_t new_pc = (getReg(instr.rs1) + instr.imm) & ~0x1u;
    setReg(instr.rd, hart.getPc() + 4);
    hart.updatePc(new_pc);
}

#define COND_BRANCH(name, condition)                    \
void Riscv::name(const Instruction& instr)              \
{                                                       \
    uint32_t new_pc = hart.getPc() + (instr.imm << 1);  \
                                                        \
    if (instr.rs1 condition instr.rs2)                  \
        hart.updatePc(new_pc);                          \
    else                                                \
        hart.updatePc();                                \
}

COND_BRANCH(beq, ==)
COND_BRANCH(bne, !=)
COND_BRANCH(blt, <)
COND_BRANCH(bge, >=)

#undef COND_BRANCH

#define UNSIGN_COND_BRANCH(name, condition)                 \
void Riscv::name(const Instruction& instr)                  \
{                                                           \
    uint32_t new_pc = hart.getPc() + (instr.imm << 1);      \
                                                            \
    if ((uint32_t)instr.rs1 condition (uint32_t)instr.rs2)  \
        hart.updatePc(new_pc);                              \
    else                                                    \
        hart.updatePc();                                    \
}

UNSIGN_COND_BRANCH(bltu, <)
UNSIGN_COND_BRANCH(bgeu, >=)

#undef UNSIGN_COND_BRANCH

/*  Load and Store instructions  */

void Riscv::lb(const Instruction& instr)
{
    address_t addr = getReg(instr.rs1) + instr.imm;
    int8_t b = 0;
    memory.read(addr, &b, sizeof(b));
    setReg(instr.rd, (int32_t)b);
    hart.updatePc();
}

void Riscv::lh(const Instruction& instr)
{
    address_t addr = getReg(instr.rs1) + instr.imm;
    int16_t h = 0;
    memory.read(addr, &h, sizeof(h));
    setReg(instr.rd, (int32_t)h);
    hart.updatePc();
}

void Riscv::lw(const Instruction& instr)
{
    address_t addr = getReg(instr.rs1) + instr.imm;
    int32_t w = 0;
    memory.read(addr, &w, sizeof(w));
    setReg(instr.rd, w);
    hart.updatePc();
}

void Riscv::lbu(const Instruction& instr)
{
    address_t addr = getReg(instr.rs1) + instr.imm;
    uint8_t ub = 0;
    memory.read(addr, &ub, sizeof(ub));
    setReg(instr.rd, (uint32_t)ub);
    hart.updatePc();
}

void Riscv::lhu(const Instruction& instr)
{
    address_t addr = getReg(instr.rs1) + instr.imm;
    uint16_t uh = 0;
    memory.read(addr, &uh, sizeof(uh));
    setReg(instr.rd, (int32_t)uh);
    hart.updatePc();
}

void Riscv::sb(const Instruction& instr)
{
    address_t addr = getReg(instr.rs1) + instr.imm;
    int8_t b = getReg(instr.rs2) & 0xFF;
    memory.write(addr, &b, sizeof(b));
    hart.updatePc();
}

void Riscv::sh(const Instruction& instr)
{
    address_t addr = getReg(instr.rs1) + instr.imm;
    int16_t h = getReg(instr.rs2) & 0xFFFF;
    memory.write(addr, &h, sizeof(h));
    hart.updatePc();
}

void Riscv::sw(const Instruction& instr)
{
    address_t addr = getReg(instr.rs1) + instr.imm;
    int32_t w = getReg(instr.rs2);
    memory.write(addr, &w, sizeof(w));
    hart.updatePc();
}
