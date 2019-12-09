#include "simulator.h"
#include <functional>


uint32_t Simulator::fetch(address_t address)
{
    uint32_t tmp = 0;
    riscv.memory.read(address, &tmp, 4);
    return tmp;
}

Instruction Simulator::decode(uint32_t encoding)
{
    return decoder.decode(encoding);
}

void Simulator::execute(const Instruction& instr)
{
    std::invoke(instr.executor, riscv, instr);
}

void Simulator::run()
{
    while(true)
    {
        uint32_t encoding = fetch(riscv.hart.pc);

        Instruction instr = decode(encoding);
        //std::cerr << "A" << (unsigned int)encoding << "\n";
        std::cerr << riscv.hart.pc << "\n\n";
        execute(instr);
    }
}
