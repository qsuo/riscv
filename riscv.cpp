
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

void Riscv::addi(const Instruction& instr)
{
    setReg(instr.rd, getReg(instr.rs1) + instr.imm);
}
