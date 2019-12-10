#define DECL_INSTR(instr_name) void instr_name(const Instruction& instr);

/*  Integer register-immediate instructions  */
DECL_INSTR(addi)
DECL_INSTR(slti)
DECL_INSTR(sltiu)
DECL_INSTR(andi)
DECL_INSTR(xori)
DECL_INSTR(ori)
DECL_INSTR(lui)
DECL_INSTR(slli)
DECL_INSTR(srli)
DECL_INSTR(srai)
DECL_INSTR(auipc)

/*  Integer register-register operations  */
DECL_INSTR(add_)
DECL_INSTR(sub)
DECL_INSTR(slt)
DECL_INSTR(sltu)
DECL_INSTR(and_)
DECL_INSTR(or_)
DECL_INSTR(xor_)
DECL_INSTR(sll)
DECL_INSTR(srl)
DECL_INSTR(sra)

/*  Control transfer instructions  */
DECL_INSTR(jal)
DECL_INSTR(jalr)
DECL_INSTR(beq)
DECL_INSTR(bne)
DECL_INSTR(blt)
DECL_INSTR(bltu)
DECL_INSTR(bge)
DECL_INSTR(bgeu)

/*  Load and Store instructions  */
DECL_INSTR(lb)
DECL_INSTR(lh)
DECL_INSTR(lw)
DECL_INSTR(lbu)
DECL_INSTR(lhu)
DECL_INSTR(sb)
DECL_INSTR(sh)
DECL_INSTR(sw)

#undef DECL_INSTR

#define DECL_INSTR(instr_name)                      \
void instr_name(const Instruction& instr)           \
{                                                   \
    assert(0 && "Instruction not implemented");     \
}

DECL_INSTR(fence)
DECL_INSTR(fence_i)
DECL_INSTR(ecall)
DECL_INSTR(ebreak)
DECL_INSTR(csrrw)
DECL_INSTR(csrrs)
DECL_INSTR(csrrc)
DECL_INSTR(csrrwi)
DECL_INSTR(csrrsi)
DECL_INSTR(csrrci)

#undef DECL_INSTR
