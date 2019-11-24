
#ifndef __RISCV_H__
#define __RISCV_H__

#include "memory.h"
#include "hart.h"
#include "instruction.h"

class Riscv
{
public:
    Riscv(uint32_t memorySize);

    std::size_t getMemorySize()
    {
        return memory.getSize();
    }

    #include "rv32i.inc"
private:
    
    Hart hart;
    Memory memory;

};

#endif
