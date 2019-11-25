
#include "../rv32i.h"
#include "../instruction.h"

int main()
{
    Instruction instr(0, 0, 0, 0, 0, 0);
    lui(&instr);
    return 0;
}
