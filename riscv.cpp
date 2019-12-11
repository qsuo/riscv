
#include <iostream>

#include <unistd.h>
#include <fcntl.h>

#include "riscv.h"

Riscv::Riscv(uint32_t memorySize):
    memory(memorySize), log(string("log_file"))
{}

size_t Riscv::getMemorySize()
{
    return memory.getSize();
}

reg_t Riscv::getReg(uint8_t num)
{
    reg_t val = hart.getReg(num);
    //logRegRead(num, val);
    return val;
}

void Riscv::setReg(uint8_t num, reg_t val)
{
    hart.setReg(num, val);
    //logRegWrite(num, val);
}

void Riscv::memRead(address_t addr, void* dst, size_t nbyte)
{
    memory.read(addr, dst, nbyte);
    //logMemRead(addr, dst, nbyte);
}

void Riscv::memWrite(address_t addr, void* src, size_t nbyte)
{
    memory.write(addr, src, nbyte);
    //logMemWrite(addr, src, nbyte);
}

#define INT_REG_IMM(name, op)                             \
void Riscv::name(const Instruction& instr)                \
{                                                         \
    setReg(instr.rd, getReg(instr.rs1) op instr.imm);     \
    hart.updatePc();                                      \
}

INT_REG_IMM(addi, +)
INT_REG_IMM(slti, <)
INT_REG_IMM(andi, &)
INT_REG_IMM(ori, |)
INT_REG_IMM(xori, ^)

#undef INT_REG_IMM

#define INT_REG_IMM(name, val)                            \
void Riscv::name(const Instruction& instr)                \
{                                                         \
    setReg(instr.rd, (val));                              \
    hart.updatePc();                                      \
}

INT_REG_IMM(sltiu, (uint32_t)(getReg(instr.rs1)) < (uint32_t)(instr.imm))

// shift instructions have shamt in rs2 field
INT_REG_IMM(slli, getReg(instr.rs1) << instr.rs2)
INT_REG_IMM(srli, (uint32_t)getReg(instr.rs1) >> instr.rs2)
INT_REG_IMM(srai, getReg(instr.rs1) >> instr.rs2)

#undef INT_REG_IMM

void Riscv::lui(const Instruction& instr)
{
    reg_t val = (instr.imm << 12) & ~0xFFF;


    setReg(instr.rd, val);
    hart.updatePc();
}

void Riscv::auipc(const Instruction& instr)
{
    // TODO: add log
    setReg(instr.rd, ((instr.imm << 12) & ~0xFFF) + hart.getPc());
    hart.updatePc();
}

/*  Integer register-register instructions  */

#define INT_REG_REG(name, op)                                  \
void Riscv::name(const Instruction& instr)                     \
{                                                              \
    setReg(instr.rd, getReg(instr.rs1) op getReg(instr.rs2));  \
    hart.updatePc();                                           \
}

INT_REG_REG(add_, +)
INT_REG_REG(sub, -)
INT_REG_REG(slt, <)
INT_REG_REG(and_, &)
INT_REG_REG(or_, |)
INT_REG_REG(xor_, ^)

#undef INT_REG_REG

#define INT_REG_REG(name, val)                            \
void Riscv::name(const Instruction& instr)                \
{                                                         \
    setReg(instr.rd, (val));                              \
    hart.updatePc();                                      \
}

INT_REG_REG(sltu, (uint32_t)getReg(instr.rs1) < (uint32_t)getReg(instr.rs2))

INT_REG_REG(sll, getReg(instr.rs1) << (getReg(instr.rs2) & 0x1F))
INT_REG_REG(srl, (uint32_t)getReg(instr.rs1) >> (getReg(instr.rs2) & 0x1F))
INT_REG_REG(sra, getReg(instr.rs1) >> (getReg(instr.rs2) & 0x1F))

#undef INT_REG_REG

/*  Control transfer instructions  */

void Riscv::jal(const Instruction& instr)
{
    uint32_t new_pc = hart.getPc() + (instr.imm << 1);
    //logJump("jal", instr.rd, new_pc);
    setReg(instr.rd, hart.getPc() + 4);
    hart.updatePc(new_pc);
}

void Riscv::jalr(const Instruction& instr)
{
    uint32_t new_pc = (getReg(instr.rs1) + instr.imm) & ~0x1u;
    //logJump("jalr", instr.rd, new_pc);
    setReg(instr.rd, hart.getPc() + 4);
    hart.updatePc(new_pc);
}

#define COND_BRANCH(name, condition)                    \
void Riscv::name(const Instruction& instr)              \
{                                                       \
    uint32_t new_pc = hart.getPc() + (instr.imm << 1);  \
                                                        \
                                                        \
    if (getReg(instr.rs1) condition getReg(instr.rs2))  \
        hart.updatePc(new_pc);                          \
    else                                                \
        hart.updatePc();                                \
}

COND_BRANCH(beq, ==)
COND_BRANCH(bne, !=)
COND_BRANCH(blt, <)
COND_BRANCH(bge, >=)

#undef COND_BRANCH

#define UNSIGN_COND_BRANCH(name, condition)                                 \
void Riscv::name(const Instruction& instr)                                  \
{                                                                           \
    uint32_t new_pc = hart.getPc() + (instr.imm << 1);                      \
                                                                            \
                                                                            \
    if ((uint32_t)getReg(instr.rs1) condition (uint32_t)getReg(instr.rs2))  \
        hart.updatePc(new_pc);                                              \
    else                                                                    \
        hart.updatePc();                                                    \
}

UNSIGN_COND_BRANCH(bltu, <)
UNSIGN_COND_BRANCH(bgeu, >=)

#undef UNSIGN_COND_BRANCH

/*  Load and Store instructions  */

void Riscv::lb(const Instruction& instr)
{
    //logRegImm("lb", instr.rd, instr.rs1, instr.imm);
    address_t addr = getReg(instr.rs1) + instr.imm;
    int8_t b = 0;
    memRead(addr, &b, sizeof(b));
    setReg(instr.rd, (int32_t)b);
    hart.updatePc();
}

void Riscv::lh(const Instruction& instr)
{
    //logRegImm("lh", instr.rd, instr.rs1, instr.imm);
    address_t addr = getReg(instr.rs1) + instr.imm;
    int16_t h = 0;
    memRead(addr, &h, sizeof(h));
    setReg(instr.rd, (int32_t)h);
    hart.updatePc();
}

void Riscv::lw(const Instruction& instr)
{
    //logRegImm("lw", instr.rd, instr.rs1, instr.imm);
    address_t addr = getReg(instr.rs1) + instr.imm;
    int32_t w = 0;
    memRead(addr, &w, sizeof(w));
    setReg(instr.rd, w);
    hart.updatePc();
}

void Riscv::lbu(const Instruction& instr)
{
    //logRegImm("lbu", instr.rd, instr.rs1, instr.imm);
    address_t addr = getReg(instr.rs1) + instr.imm;
    uint8_t ub = 0;
    memRead(addr, &ub, sizeof(ub));
    setReg(instr.rd, (uint32_t)ub);
    hart.updatePc();
}

void Riscv::lhu(const Instruction& instr)
{
    //logRegImm("lhu", instr.rd, instr.rs1, instr.imm);
    address_t addr = getReg(instr.rs1) + instr.imm;
    uint16_t uh = 0;
    memRead(addr, &uh, sizeof(uh));
    setReg(instr.rd, (int32_t)uh);
    hart.updatePc();
}

void Riscv::sb(const Instruction& instr)
{
    //logRegImm("sb", instr.rs2, instr.rs1, instr.imm);
    address_t addr = getReg(instr.rs1) + instr.imm;
    int8_t b = getReg(instr.rs2) & 0xFF;
    memWrite(addr, &b, sizeof(b));
    hart.updatePc();
}

void Riscv::sh(const Instruction& instr)
{
    //logRegImm("sh", instr.rs2, instr.rs1, instr.imm);
    address_t addr = getReg(instr.rs1) + instr.imm;
    int16_t h = getReg(instr.rs2) & 0xFFFF;
    memWrite(addr, &h, sizeof(h));
    hart.updatePc();
}

void Riscv::sw(const Instruction& instr)
{
    //logRegImm("sw", instr.rs2, instr.rs1, instr.imm);
    address_t addr = getReg(instr.rs1) + instr.imm;
    int32_t w = getReg(instr.rs2);
    memWrite(addr, &w, sizeof(w));
    hart.updatePc();
}

/*  Log service  */

void Riscv::logRegRead(uint8_t num, reg_t val)
{
    log << "RR " << regToStr(num) << " : " << immToStr(val) << endl;
}

void Riscv::logRegWrite(uint8_t num, reg_t val)
{
    log << "RW " << regToStr(num) << " : " << immToStr(val) << endl;
}

void Riscv::logMemRead(address_t addr, void* dst, size_t nbyte)
{
    log << "MR";
    switch (nbyte)
    {
        case sizeof(uint8_t):
            log << "B " << addrToStr(addr) << " : " << immToStr(*(uint8_t*)dst);
            break;
        case sizeof(uint16_t):
            log << "H " << addrToStr(addr) << " : " << immToStr(*(uint16_t*)dst);
            break;
        case sizeof(uint32_t):
            log << "W " << addrToStr(addr) << " : " << immToStr(*(uint32_t*)dst);
            break;
        default:
            assert(0 && "Invalid memory read size");
    }
    log << endl;
}

void Riscv::logMemWrite(address_t addr, void* src, size_t nbyte)
{
    log << "MW";
    switch (nbyte)
    {
        case sizeof(uint8_t):
            log << "B " << addrToStr(addr) << " : " << immToStr(*(uint8_t*)src);
            break;
        case sizeof(uint16_t):
            log << "H " << addrToStr(addr) << " : " << immToStr(*(uint16_t*)src);
            break;
        case sizeof(uint32_t):
            log << "W " << addrToStr(addr) << " : " << immToStr(*(uint32_t*)src);
            break;
        default:
            assert(0 && "Invalid memory read size");
    }
    log << endl;
}

void Riscv::logRegImm(string name, uint8_t r1, uint8_t r2, int32_t imm)
{
    log << addrToStr(hart.getPc()) << " " << name << " " << regToStr(r1) << ", "
        << regToStr(r2) << ", " << immToStr(imm) << endl;
}

void Riscv::logIntRegReg(string name, uint8_t rd, uint8_t rs1, uint8_t rs2)
{
    log << addrToStr(hart.getPc()) << " " << name << " " << regToStr(rd) << ", "
        << regToStr(rs1) << ", " << regToStr(rs2) << endl;
}

void Riscv::logJump(string name, uint8_t rd, uint32_t addr)
{
    log << addrToStr(hart.getPc()) << " " << name
        << regToStr(rd) << ", " << addrToStr(addr) << endl;
}

void Riscv::logCondBr(string name, uint8_t rs1, uint8_t rs2, uint32_t addr)
{
    log << addrToStr(hart.getPc()) << " " << name << " " << regToStr(rs1)
        << ", " << regToStr(rs2) << ", " << addrToStr(addr) << endl;
}
