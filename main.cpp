
#include "simulator.h"
#include <iostream>
#include <stdio.h>

#define SIZE 10000000

int main()
{

    Simulator sim(SIZE);
    uint32_t tmp = 0b0100000'00000'00000'000'00010'0110011; //sub
    sim.riscv.memory.write(10, &tmp, sizeof(tmp));
    tmp = 0b0100000'00000'00000'010'00010'0000011; //sub
    sim.riscv.memory.write(14, &tmp, sizeof(tmp));
    sim.riscv.hart.pc = 10;

    std::ifstream F;
    uint32_t instruction = 0;
    uint32_t address = 0;
    F.open("ptload.txt", std::ios::in);
    if (F) {
        F >> sim.riscv.hart.pc;
        while (!F.eof()) {
            F >> address >> instruction;
            sim.riscv.memory.write(address, &instruction, sizeof(instruction));
        }
    }
    //pc = startAddress;
    //std::cout << pc << std::endl;

    sim.run();
    return 0;
}
