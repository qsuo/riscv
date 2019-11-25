
#ifndef __INSTRUCTION_H__
#define __INSTRUCTION_H__

#include <stdint.h>

class Riscv;

class Instruction
{
public:

    typedef void (Riscv::*Executor)(const Instruction& instr);

    Instruction(uint32_t encoding, Executor executor,
                uint16_t imm, uint8_t rs1, uint8_t rs2, uint8_t rd):
        encoding(encoding),
        imm(imm),
        rs1(rs1),
        rs2(rs2),
        rd(rd),
        executor(executor)
    {}

    uint32_t encoding;
    uint16_t imm;
    uint8_t rs1;
    uint8_t rs2;
    uint8_t rd;


private:
    Executor executor;
};

#endif
