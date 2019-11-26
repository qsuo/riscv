
#ifndef __RISCV_H__
#define __RISCV_H__

#include "memory.h"
#include "hart.h"
#include "instruction.h"

class Riscv
{
public:
    Riscv(uint32_t memorySize);

    size_t getMemorySize();
    reg_t getReg(int num);
    void setReg(int num, reg_t val);

    // instructions executors
    #include "rv32i.inc"
private:

    Hart hart;
    Memory memory;
};

#endif
