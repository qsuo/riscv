
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

    //Instruction* decode(uint32_t encoding);
    
    static const int BYTE_SIZE = 8;

    uint32_t applyMask(uint32_t raw, uint32_t mask);
    
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
    };

    Decoding getDecoding(uint32_t encoding);
    


private:
    int findFirstUp(uint32_t raw);

};

#endif
