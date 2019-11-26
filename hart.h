
#ifndef __HART_H__
#define __HART_H__

#include <array>

#include "memory.h"

typedef uint32_t reg_t;

class Hart
{
public:
    void updatePc(uint32_t newPc=0); //TODO address_t
    reg_t getReg(int num);
    void setReg(int num, reg_t val);

private:
    static const int XLEN = 32;
    uint32_t pc;
    std::array<reg_t, XLEN> regs;
};

#endif
