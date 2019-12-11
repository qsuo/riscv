
#ifndef __SIMULATOR_H__
#define __SIMULATOR_H__

#include "riscv.h"
#include "decoder.h"
#include "instruction.h"
#include "basic_block.h"

#include <unordered_map>

class Simulator
{
public:
    Simulator(size_t memorySize):riscv(memorySize)
    {}
    ~Simulator();
    void load(const char* name);

    uint32_t fetch(address_t address);
    Instruction decode(uint32_t encoding);
    void execute(const Instruction& instr);

    void execute(BasicBlock* bb);

    BasicBlock* getBasicBlock();

    void run();
    void dump() {}

    std::unordered_map<uint32_t, BasicBlock*> map;
//private:
    Riscv riscv;
    Decoder decoder;
};

#endif

