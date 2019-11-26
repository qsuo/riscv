#include "simulator.h"
#include <functional>

uint32_t Simulator::fetch(address_t address) {}

Instruction Simulator::decode(uint32_t encoding)
{
    return decoder.decode(encoding);
}

void Simulator::execute(const Instruction& instr)
{
    std::invoke(instr.executor, riscv, instr);
}
