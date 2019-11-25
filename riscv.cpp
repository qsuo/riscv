
#include <iostream>

#include <unistd.h>
#include <fcntl.h>

#include "riscv.h"

Riscv::Riscv(uint32_t memorySize):
    memory(memorySize)
{}

void Riscv::addi(const Instruction& instr)
{
  hart.reg[instr.rd] = hart.reg[instr.rs1] + instr.imm;
}
