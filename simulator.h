
#ifndef __SIMULATOR_H__
#define __SIMULATOR_H__

#include "riscv.h"
#include "decoder.h"
#include "instruction.h"

class Simulator
{
public:
    Simulator(size_t memorySize):riscv(memorySize)
    {}

    uint32_t fetch(address_t address);
    Instruction decode(uint32_t encoding);
    void execute(const Instruction& instr);

    void run();
//private:
    Riscv riscv;
    Decoder decoder;
};

#endif

