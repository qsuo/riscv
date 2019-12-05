
#include <iostream>

#include <unistd.h>
#include <fcntl.h>

#include "riscv.h"

Riscv::Riscv(uint32_t memorySize):
    memory(memorySize)
{}

size_t Riscv::getMemorySize()
{
    return memory.getSize();
}

reg_t Riscv::getReg(int num)
{
    return hart.getReg(num);
}

void Riscv::setReg(int num, reg_t val)
{
    hart.setReg(num, val);
}

/*  Integer register-immediate instructions  */

void Riscv::addi(const Instruction& instr)
{
    setReg(instr.rd, getReg(instr.rs1) + instr.imm);
    hart.updatePc();
}

void Riscv::slti(const Instruction& instr)
{
    setReg(instr.rd, getReg(instr.rs1) < instr.imm);
    hart.updatePc();
}

void Riscv::sltiu(const Instruction& instr)
{
    setReg(instr.rd, (uint32_t)(getReg(instr.rs1)) < (uint32_t)(instr.imm));
    hart.updatePc();
}

void Riscv::andi(const Instruction& instr)
{
    setReg(instr.rd, getReg(instr.rs1) & instr.imm);
    hart.updatePc();
}

void Riscv::ori(const Instruction& instr)
{
    setReg(instr.rd, getReg(instr.rs1) | instr.imm);
    hart.updatePc();
}

void Riscv::xori(const Instruction& instr)
{
    setReg(instr.rd, getReg(instr.rs1) ^ instr.imm);
    hart.updatePc();
}

void Riscv::slli(const Instruction& instr)
{
    setReg(instr.rd, getReg(instr.rs1) << (instr.imm & 0x1F));
    hart.updatePc();
}

void Riscv::srli(const Instruction& instr)
{
    setReg(instr.rd, (uint32_t)getReg(instr.rs1) >> (instr.imm & 0x1F));
    hart.updatePc();
}

void Riscv::srai(const Instruction& instr)
{
    setReg(instr.rd, getReg(instr.rs1) >> (instr.imm & 0x1F));
    hart.updatePc();
}

void Riscv::lui(const Instruction& instr)
{
    setReg(instr.rd, (instr.imm << 12) & ~0xFFF);
    hart.updatePc();
}

void Riscv::auipc(const Instruction& instr)
{
    reg_t res = ((instr.imm << 12) & ~0xFFF) + hart.getPc();
    setReg(instr.rd, res);
    hart.updatePc();
}

/*  Integer register-register instructions  */

void Riscv::add_(const Instruction& instr)
{
    setReg(instr.rd, getReg(instr.rs1) + getReg(instr.rs2));
    hart.updatePc();
}

void Riscv::sub(const Instruction& instr)
{
    setReg(instr.rd, getReg(instr.rs1) - getReg(instr.rs2));
    hart.updatePc();
}

void Riscv::slt(const Instruction& instr)
{
    setReg(instr.rd, getReg(instr.rs1) < getReg(instr.rs2));
    hart.updatePc();
}

void Riscv::sltu(const Instruction& instr)
{
    setReg(instr.rd, (uint32_t)getReg(instr.rs1) < (uint32_t)getReg(instr.rs2));
    hart.updatePc();
}

void Riscv::and_(const Instruction& instr)
{
    setReg(instr.rd, getReg(instr.rs1) & getReg(instr.rs2));
    hart.updatePc();
}

void Riscv::or_(const Instruction& instr)
{
    setReg(instr.rd, getReg(instr.rs1) | getReg(instr.rs2));
    hart.updatePc();
}

void Riscv::xor_(const Instruction& instr)
{
    setReg(instr.rd, getReg(instr.rs1) ^ getReg(instr.rs2));
    hart.updatePc();
}

void Riscv::sll(const Instruction& instr)
{
    setReg(instr.rd, getReg(instr.rs1) << (getReg(instr.rs2) & 0x1F));
    hart.updatePc();
}

void Riscv::srl(const Instruction& instr)
{
    setReg(instr.rd, (uint32_t)getReg(instr.rs1) >> (getReg(instr.rs2) & 0x1F));
    hart.updatePc();
}

void Riscv::sra(const Instruction& instr)
{
    setReg(instr.rd, getReg(instr.rs1) >> (getReg(instr.rs2) & 0x1F));
    hart.updatePc();
}

/*  Control transfer instructions  */

void Riscv::jal(const Instruction& instr)
{
    uint32_t new_pc = hart.getPc() + (instr.imm << 1);
    setReg(instr.rd, hart.getPc() + 4);
    hart.updatePc(new_pc);
}

void Riscv::jalr(const Instruction& instr)
{
    uint32_t new_pc = (getReg(instr.rs1) + instr.imm) & ~0x1u;
    setReg(instr.rd, hart.getPc() + 4);
    hart.updatePc(new_pc);
}

void Riscv::beq(const Instruction& instr)
{
    uint32_t new_pc = hart.getPc() + (instr.imm << 1);

    if (instr.rs1 == instr.rs2)
        hart.updatePc(new_pc);
    else
        hart.updatePc();
}

void Riscv::bne(const Instruction& instr)
{
    uint32_t new_pc = hart.getPc() + (instr.imm << 1);

    if (instr.rs1 != instr.rs2)
        hart.updatePc(new_pc);
    else
        hart.updatePc();
}

void Riscv::blt(const Instruction& instr)
{
    uint32_t new_pc = hart.getPc() + (instr.imm << 1);

    if (instr.rs1 < instr.rs2)
        hart.updatePc(new_pc);
    else
        hart.updatePc();
}

void Riscv::bltu(const Instruction& instr)
{
    uint32_t new_pc = hart.getPc() + (instr.imm << 1);

    if ((uint32_t)instr.rs1 < (uint32_t)instr.rs2)
        hart.updatePc(new_pc);
    else
        hart.updatePc();
}

void Riscv::bge(const Instruction& instr)
{
    uint32_t new_pc = hart.getPc() + (instr.imm << 1);

    if (instr.rs1 >= instr.rs2)
        hart.updatePc(new_pc);
    else
        hart.updatePc();
}

void Riscv::bgeu(const Instruction& instr)
{
    uint32_t new_pc = hart.getPc() + (instr.imm << 1);

    if ((uint32_t)instr.rs1 >= (uint32_t)instr.rs2)
        hart.updatePc(new_pc);
    else
        hart.updatePc();
}

void Riscv::lb(const Instruction& instr)
{
    address_t addr = getReg(instr.rs1) + instr.imm;
    int8_t b = 0;
    memory.read(addr, &b, sizeof(b));
    setReg(instr.rd, (int32_t)b);
    hart.updatePc();
}

void Riscv::lh(const Instruction& instr)
{
    address_t addr = getReg(instr.rs1) + instr.imm;
    int16_t h = 0;
    memory.read(addr, &h, sizeof(h));
    setReg(instr.rd, (int32_t)h);
    hart.updatePc();
}

void Riscv::lw(const Instruction& instr)
{
    address_t addr = getReg(instr.rs1) + instr.imm;
    int32_t w = 0;
    memory.read(addr, &w, sizeof(w));
    setReg(instr.rd, w);
    hart.updatePc();
}

void Riscv::lbu(const Instruction& instr)
{
    address_t addr = getReg(instr.rs1) + instr.imm;
    uint8_t ub = 0;
    memory.read(addr, &ub, sizeof(ub));
    setReg(instr.rd, (uint32_t)ub);
    hart.updatePc();
}

void Riscv::lhu(const Instruction& instr)
{
    address_t addr = getReg(instr.rs1) + instr.imm;
    uint16_t uh = 0;
    memory.read(addr, &uh, sizeof(uh));
    setReg(instr.rd, (int32_t)uh);
    hart.updatePc();
}

void Riscv::sb(const Instruction& instr)
{
    address_t addr = getReg(instr.rs1) + instr.imm;
    int8_t b = getReg(instr.rs2) & 0xFF;
    memory.write(addr, &b, sizeof(b));
    hart.updatePc();
}

void Riscv::sh(const Instruction& instr)
{
    address_t addr = getReg(instr.rs1) + instr.imm;
    int16_t h = getReg(instr.rs2) & 0xFFFF;
    memory.write(addr, &h, sizeof(h));
    hart.updatePc();
}

void Riscv::sw(const Instruction& instr)
{
    address_t addr = getReg(instr.rs1) + instr.imm;
    int32_t w = getReg(instr.rs2);
    memory.write(addr, &w, sizeof(w));
    hart.updatePc();
}
