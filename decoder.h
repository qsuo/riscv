#ifndef __DECODER_H__
#define __DECODER_H__

#include <stdint.h>
#include <unordered_map>

//#include "instruction.h"
#include "riscv.h"


class Decoder
{
public:
    //Decoder();
    //~Decoder();

    typedef void (Riscv::*Executor)(const Instruction& instr);


    enum Type
    {
        R = 1,
        I,
        S,
        B,
        U,
        J,
        NONE
    };

    enum DecodingType
    {
        RALU = 0b0110011,
        RIALU = 0b0010011,
        PROC = 0b1110011,
        LUI = 0b0110111,
        AUIPC = 0b0010111,
        JAL = 0b1101111,
        JALR = 0b1100111,
        SR = 0b101
    };

    std::unordered_map<uint32_t, Type> instrType = {
        {0b0110111, U},
        {0b0010111, U},
        {0b1101111, J},
        {0b1100111, I},
        {0b1100011, B},
        {0b0000011, I},
        {0b0100011, S},
        {0b0010011, I},
        {0b0110011, R},
        {0b1110011, I},
        {0b0001111, NONE}};


    typedef std::unordered_map<uint32_t, Executor> instrMap;
    instrMap IAlu = {
        {0b000, &Riscv::addi},
        {0b010, &Riscv::slti},
        {0b011, &Riscv::sltiu},
        {0b100, &Riscv::xori},
        {0b110, &Riscv::ori},
        {0b111, &Riscv::andi},
        {0b001, &Riscv::slli},
        {0b1010, &Riscv::srli},
        {0b1011, &Riscv::srai},
    };

    instrMap RAlu = {
        {0b0000000000, &Riscv::add_},
        {0b0100000000, &Riscv::sub},
        {0b0000000001, &Riscv::sll},
        {0b0000000010, &Riscv::slt},
        {0b0000000011, &Riscv::sltu},
        {0b0000000100, &Riscv::xor_},
        {0b0000000101, &Riscv::srl},
        {0b0100000101, &Riscv::sra},
        {0b0000000110, &Riscv::or_},
        {0b0000000111, &Riscv::and_}};

    instrMap Branch = {
        {0b000, &Riscv::beq},
        {0b001, &Riscv::bne},
        {0b100, &Riscv::blt},
        {0b101, &Riscv::bge},
        {0b110, &Riscv::bltu},
        {0b111, &Riscv::bgeu}};

    instrMap Load = {
        {0b000, &Riscv::lb},
        {0b001, &Riscv::lh},
        {0b010, &Riscv::lw},
        {0b100, &Riscv::lbu},
        {0b101, &Riscv::lhu}};

    instrMap Store = {
        {0b000, &Riscv::sb},
        {0b001, &Riscv::sh},
        {0b010, &Riscv::sw}};

    instrMap Jump = {
        {0b0, &Riscv::jal}};

    instrMap RJump = {
        {0b0, &Riscv::jalr}};

    instrMap LoadUpper = {
        {0b0, &Riscv::lui}};

    instrMap AddPc = {
        {0b0, &Riscv::auipc}};

    instrMap Fence = {
        {0b000, &Riscv::fence},
        {0b001, &Riscv::fence_i}};

    instrMap Procedure = {
        {0b0, &Riscv::ecall},
        {0b1, &Riscv::ebreak}};

    std::unordered_map<uint32_t, instrMap> instructions = {
        {0b0110111, LoadUpper},
        {0b0010111, AddPc},
        {0b1101111, Jump},
        {0b1100111, RJump},
        {0b1100011, Branch},
        {0b0000011, Load},
        {0b0100011, Store},
        {0b0010011, IAlu},
        {0b0110011, RAlu},
        {0b0001111, Fence},
        {0b1110011, Procedure}};

    static const int BYTE_SIZE = 8;

    struct Mask
    {
        enum Field
        {
            OPCODE      = 0b00000000'00000000'00000000'01111111,
            RD          = 0b00000000'00000000'00001111'10000000,
            FUNCT3      = 0b00000000'00000000'01110000'00000000,
            RS1         = 0b00000000'00001111'10000000'00000000,
            RS2         = 0b00000001'11110000'00000000'00000000,
            FUNCT7      = 0b11111110'00000000'00000000'00000000,
            I_IMM11_0   = 0b11111111'11110000'00000000'00000000,
            S_IMM11_5   = 0b11111110'00000000'00000000'00000000,
            S_IMM4_0    = 0b00000000'00000000'00001111'10000000,
            B_IMM12     = 0b10000000'00000000'00000000'00000000,
            B_IMM10_5   = 0b01111110'00000000'00000000'00000000,
            B_IMM4_1    = 0b00000000'00000000'00001111'00000000,
            B_IMM11     = 0b00000000'00000000'00000000'10000000,
            U_IMM31_12  = 0b11111111'11111111'11110000'00000000,
            J_IMM20     = 0b10000000'00000000'00000000'00000000,
            J_IMM10_1   = 0b01111111'11100000'00000000'00000000,
            J_IMM11     = 0b00000000'00010000'00000000'00000000,
            J_IMM19_12  = 0b00000000'00001111'11110000'00000000
        };
    };

    struct Decoding
    {
        uint32_t opcode;
        uint32_t rd;
        uint32_t funct3;
        uint32_t rs1;
        uint32_t rs2;
        uint32_t funct7;
        uint32_t I_imm11_0;
        uint32_t S_imm11_5;
        uint32_t S_imm4_0;
        uint32_t B_imm12;
        uint32_t B_imm10_5;
        uint32_t B_imm4_1;
        uint32_t B_imm11;
        uint32_t U_imm31_12;
        uint32_t J_imm20;
        uint32_t J_imm10_1;
        uint32_t J_imm11;
        uint32_t J_imm19_12;

        uint32_t getIImm();
        uint32_t getSImm();
        uint32_t getBImm();
        uint32_t getUImm();
        uint32_t getJImm();

        uint32_t getImm(Type type);
    };

    uint32_t applyMask(uint32_t raw, uint32_t mask);
    Decoding getDecoding(uint32_t encoding);
    Instruction decode(uint32_t encoding);

private:
    int findFirstUp(uint32_t raw);

};

#endif
