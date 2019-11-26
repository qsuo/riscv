
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

void Riscv::addi(const Instruction& instr)
{
    setReg(instr.rd, getReg(instr.rs1) + instr.imm);
}

void Riscv::slti(const Instruction& instr)
{
    setReg(instr.rd, getReg(instr.rs1) < instr.imm);
}

void Riscv::sltiu(const Instruction& instr)
{
    bool res = (uint32_t)((int32_t)getReg(instr.rs1)) <
               (uint32_t)((int32_t)instr.imm);
    setReg(instr.rd, res);
}

void Riscv::andi(const Instruction& instr)
{
    setReg(instr.rd, getReg(instr.rs1) & instr.imm);
}

void Riscv::ori(const Instruction& instr)
{
    setReg(instr.rd, getReg(instr.rs1) | instr.imm);
}

void Riscv::xori(const Instruction& instr)
{
    setReg(instr.rd, getReg(instr.rs1) ^ instr.imm);
}

void Riscv::slli(const Instruction& instr)
{
    setReg(instr.rd, getReg(instr.rs1) << (instr.imm & 0x1F));
}

void Riscv::srli(const Instruction& instr)
{
    setReg(instr.rd, (uint32_t)getReg(instr.rs1) >> (instr.imm & 0x1F));
}

void Riscv::srai(const Instruction& instr)
{
    setReg(instr.rd, getReg(instr.rs1) >> (instr.imm & 0x1F));
}

void Riscv::lui(const Instruction& instr)
{
    setReg(instr.rd, ((uint32_t)instr.imm << 12) & ~0xFFF); // not sure
}

void Riscv::auipc(const Instruction& instr)
{
    // not sure
    reg_t res = (((uint32_t)instr.imm << 12) & ~0xFFF) + hart.getPc();
    setReg(instr.rd, res);
}
