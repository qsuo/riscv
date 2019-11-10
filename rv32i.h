
#ifndef __RV32I_H__
#define __RV32I_H__

#include <stdint.h>

#include "instruction.h"

#define DEF_INSTR(instr) int instr(Insctruction& this, Instruction& next)

namespace rv32i
{
    DEF_INSTR(lui);
    DEF_INSTR(auipc);
    DEF_INSTR(jal);
    DEF_INSTR(jalr);
    DEF_INSTR(beq);
    DEF_INSRT(bne);
    DEF_INSRT(blt);
    DEF_INSRT(bge);
    DEF_INSRT(bltu);
    DEF_INSRT(bgeu);
    DEF_INSRT(lb);
    DEF_INSRT(lh);
    DEF_INSRT(lw);
    DEF_INSRT(lbu);
    DEF_INSRT(lhu);
    DEF_INSRT(sb);
    DEF_INSRT(sh);
    DEF_INSRT(sw);
    DEF_INSRT(addi);
    DEF_INSRT(slti);
    DEF_INSRT(sltiu);
    DEF_INSRT(xori);
    DEF_INSRT(ori);
    DEF_INSRT(andi);
    DEF_INSRT(slli);
    DEF_INSRT(srai);
    DEF_INSRT(add);
    DEF_INSRT(sub);
    DEF_INSRT(sll);
    DEF_INSRT(slt);
    DEF_INSRT(sltu);
    DEF_INSRT(xor);
    DEF_INSRT(srl);
    DEF_INSRT(sra);
    DEF_INSRT(or);
    DEF_INSRT(and);
    DEF_INSRT(fence);
    DEF_INSRT(fence_i);
    DEF_INSRT(ecall);
    DEF_INSRT(ebreak);
    DEF_INSRT(csrrw);
    DEF_INSRT(csrrs);
    DEF_INSRT(csrrc);
    DEF_INSRT(csrrwi);
    DEF_INSRT(csrrsi);
    DEF_INSRT(csrrci);
}

#undef DEF_INSTR

#endif
