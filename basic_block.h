
#ifndef __BASIC_BLOCK_H__
#define __BASIC_BLOCK_H__

#include <array>

#include "instruction.h"

class BasicBlock
{
public:
    const static int BLOCK_SIZE = 16;

    void execute()
    {
        for(auto instr : instructions)
            std::invoke(instr.executor, riscv, instr);

    }
//private:
    std::array<Instruction, BLOCK_SIZE> instructions;

};

#endif

