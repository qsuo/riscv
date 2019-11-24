
#ifndef __HART_H__
#define __HART_H__

#include "memory.h"

typedef uint32_t reg_t;

class Hart
{
private:
    static const int XLEN = 32;
public:
    void updatePc(uint32_t newPc=0); //TODO address_t
    reg_t reg[XLEN];
private:
    uint32_t pc;
};

#endif
