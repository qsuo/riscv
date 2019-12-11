
#include "basic_block.h"

BasicBlock::~BasicBlock()
{
    for(int i = 0; i < count; i++)
        delete instructions[i];
}

void BasicBlock::add(Instruction* instr)
{
    instructions[count++] = instr;
}

void BasicBlock::execute(Riscv& riscv)
{
    for(int i = 0; i < count; i++)
    {
        std::invoke(instructions[i]->executor, riscv, *instructions[i]);
    }
}

