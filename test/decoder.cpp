
#include "../decoder.h"
#include "../riscv.h"
#include "../instruction.h"
#include <iostream>
#include <cassert>

#define TEST_DECODE(encoding, func) \
    do \
    { \
        instr = decoder.decode(encoding); \
        assert(instr.executor == &Riscv::func); \
    } while(0)



int main()
{
    Decoder decoder;
    //int encoding = 0b10101010'10101010'10101010'10101010;

    int encoding = 0b11101010'10101010'10101110'10101010;

    Decoder::Decoding decoding = decoder.getDecoding(encoding);
    /*
    assert(decoding.opcode == 0b101010);
    assert(decoding.rd == (encoding & 0xf80) >> 7);
    assert(decoding.funct3 == 0b010);
    assert(decoding.rs1 == (encoding & 0xf8000) >> 15);
    assert(decoding.rs2 == (encoding & 0x1f00000) >> 20);
    assert(decoding.funct7 == 0b1010101);
    assert(decoding.getIImm() == 0b10101010'1010);
    */
    assert(decoding.getSImm() == 0b111010111101);
    assert(decoding.getBImm() == 0b111101011110);
    assert(decoding.getUImm() == 0b11101010101010101010);
    assert(decoding.getJImm() == 0b11010101001101010101);

    assert(decoding.getImm(Decoder::S) == 0b111010111101);

    Instruction instr = decoder.decode(0b0110111);
    
    try
    {
        instr = decoder.decode(1);
        assert(0);
    }
    catch(std::out_of_range ex)
    {}


    TEST_DECODE(0b0110111, lui);
    TEST_DECODE(0b0010111, auipc);
    TEST_DECODE(0b1101111, jal);
    TEST_DECODE(0b1100111, jalr);

    TEST_DECODE(0b000'00000'1100011, beq);
    TEST_DECODE(0b001'00000'1100011, bne);
    TEST_DECODE(0b100'00000'1100011, blt);
    TEST_DECODE(0b101'00000'1100011, bge);
    TEST_DECODE(0b110'00000'1100011, bltu);
    TEST_DECODE(0b111'00000'1100011, bgeu);
 
    TEST_DECODE(0b000'00000'0000011, lb);
    TEST_DECODE(0b001'00000'0000011, lh);
    TEST_DECODE(0b010'00000'0000011, lw);
    TEST_DECODE(0b100'00000'0000011, lbu);
    TEST_DECODE(0b101'00000'0000011, lhu);

    TEST_DECODE(0b000'00000'0100011, sb);
    TEST_DECODE(0b001'00000'0100011, sh);
    TEST_DECODE(0b010'00000'0100011, sw);

    TEST_DECODE(0b000'00000'0010011, addi);
    TEST_DECODE(0b010'00000'0010011, slti);
    TEST_DECODE(0b011'00000'0010011, sltiu);
    TEST_DECODE(0b100'00000'0010011, xori);
    TEST_DECODE(0b110'00000'0010011, ori);
    TEST_DECODE(0b111'00000'0010011, andi);
    TEST_DECODE(0b001'00000'0010011, slli);
    TEST_DECODE(0b0000000'00000'00000'101'00000'0010011, srli);
    TEST_DECODE(0b0100000'00000'00000'101'00000'0010011, srai);


    TEST_DECODE(0b0000000'00000'00000'000'00000'0110011, add_);
    TEST_DECODE(0b0100000'00000'00000'000'00000'0110011, sub);
    TEST_DECODE(0b0000000'00000'00000'001'00000'0110011, sll);
    TEST_DECODE(0b0000000'00000'00000'010'00000'0110011, slt);
    TEST_DECODE(0b0000000'00000'00000'011'00000'0110011, sltu);
    TEST_DECODE(0b0000000'00000'00000'100'00000'0110011, xor_);
    TEST_DECODE(0b0000000'00000'00000'101'00000'0110011, srl);
    TEST_DECODE(0b0100000'00000'00000'101'00000'0110011, sra);
    TEST_DECODE(0b0000000'00000'00000'110'00000'0110011, or_);
    TEST_DECODE(0b0000000'00000'00000'111'00000'0110011, and_);

    TEST_DECODE(0b000'00000'0001111, fence);
    TEST_DECODE(0b001'00000'0001111, fence_i);
    
    TEST_DECODE(0b0'00000'000'00000'1110011, ecall);
    TEST_DECODE(0b1'00000'000'00000'1110011, ebreak);


    return 0;
}




