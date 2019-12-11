
#ifndef __BASIC_BLOCK_H__
#define __BASIC_BLOCK_H__

#include <array>
#include <functional>

#include "instruction.h"
#include "riscv.h"

class BasicBlock
{
public:
    BasicBlock(): count(0)
    {}

    ~BasicBlock();

    static const int BLOCK_SIZE = 16;
    uint32_t start;
    uint32_t next;

    int count;

    std::array<Instruction*, BLOCK_SIZE> instructions;
    void execute(Riscv& riscv);   
    void add(Instruction* instr);
};

#endif
