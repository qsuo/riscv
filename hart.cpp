
#include "hart.h"

void Hart::updatePc(uint32_t newPc /*=0*/)
{
    if(newPc == 0)
        pc += 4 ;
    else
        pc = newPc;
}
