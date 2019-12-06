
#ifndef __RISCV_H__
#define __RISCV_H__

#include <iostream>
#include <fstream>
#include <iomanip>

#include "memory.h"
#include "hart.h"
#include "instruction.h"
#include "log_service.h"


class Riscv
{
public:
    Riscv(uint32_t memorySize);

    size_t getMemorySize();
    reg_t getReg(int num);
    void setReg(int num, reg_t val);

    // instructions executors
    #include "rv32i.inc"

private:
    void logRegRead(uint8_t num, reg_t val);
    void logRegWrite(uint8_t num, reg_t val);
    void logRegImm(string name, uint8_t rd, uint8_t rs1, int32_t imm);
    void logIntRegReg(string name, uint8_t rd, uint8_t rs1, uint8_t rs2);
    void logJump(string name, uint8_t rd, uint32_t addr);
    void logCondBr(string name, uint8_t rs1, uint8_t rs2, uint32_t addr);

    Hart hart;
    Memory memory;
    ofstream log;
};

#endif
