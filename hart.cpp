
#include <cassert>

#include "hart.h"

void Hart::updatePc(uint32_t newPc /*=0*/)
{
    if(newPc == 0)
        pc += 4 ;
    else
        pc = newPc;
}

reg_t Hart::getReg(int num)
{
    assert(num < XLEN);
    return regs[num];
}

void Hart::setReg(int num, reg_t val)
{
    assert(num < XLEN);
    regs[num] = val;
}
