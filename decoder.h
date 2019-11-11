
#ifndef __DECODER_H__
#define __DECODER_H__

#include <stdint.h>

//#include "instruction.h"
#include <map>


class Decoder
{
public:
    //Decoder();
    //~Decoder();

  //  Instruction* decode(uint32_t encoding);
    
    static const int BYTE_SIZE = 8;

    uint32_t applyMask(uint32_t raw, uint32_t mask);

    enum Field
    {
        OPCODE,
        RD,
        FUNCT3,
        RS1,
        RS2, 
        FUNCT7,
        I_IMM11_0,
        S_IMM11_5,
        S_IMM4_0,
        B_IMM12,
        B_IMM10_5,
        B_IMM4_1,
        B_IMM11,
        U_IMM31_12,
        J_IMM20,
        J_IMM10_1,
        J_IMM11,
        J_IMM19_12
    };

    struct Decoding
    {
        int opcode;
        int rd;
        int funct3;
        int rs1;
        int rs2;
        int funct7;
        int I_imm11_0;
        int S_imm11_5;
        int S_imm4_0;
        int B_imm12;
        int B_imm10_5;
        int B_imm4_1;
        int B_imm11;
        int U_imm31_12;
        int J_imm20;
        int J_imm10_1;
        int J_imm11;
        int J_imm19_12;
    };

    Decoding decode(uint32_t encoding);

    int findFirstUp(uint32_t raw);
    std::map<Field, uint32_t>  masks =  {   {OPCODE,    0b00000000'00000000'00000000'01111111},
                                            {RD,        0b00000000'00000000'00001111'10000000},
                                            {FUNCT3,    0b00000000'00000000'00011000'00000000},
                                            {RS1,       0b00000000'00000000' }
                                        };

};

#endif
