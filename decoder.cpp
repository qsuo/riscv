
#include <iostream>
#include <cassert>

#include "decoder.h"


int Decoder::findFirstUp(uint32_t raw)
{
    int bit = 0;
    uint32_t mask = 0x1;
    int nbit = sizeof(raw) * BYTE_SIZE;
    int pos = 0;
    for(pos = 0; pos < nbit && bit == 0; pos++)
    {
        int newMask = mask << pos;
        bit = (raw & newMask) >> pos;
    }

    return --pos;
}

uint32_t Decoder::applyMask(uint32_t raw, uint32_t mask)
{
    return (raw & mask) >> this->findFirstUp(mask);
}

int32_t Decoder::Decoding::getIImm()
{
    return (int32_t)I_imm11_0;
}

int32_t Decoder::Decoding::getSImm()
{
    uint32_t imm4_0 = S_imm4_0;
    uint32_t imm11_5 = S_imm11_5 << 5;
    int32_t imm = imm4_0 | imm11_5;
    return imm;
}

int32_t Decoder::Decoding::getBImm()
{
    uint32_t imm12 = B_imm12 << 11;
    uint32_t imm10_5 = B_imm10_5 << 4;
    uint32_t imm4_1 = B_imm4_1;
    uint32_t imm11 = B_imm11 << 10;
    int32_t imm = imm4_1 | imm10_5 | imm11 | imm12;
    return imm;
}

int32_t Decoder::Decoding::getUImm()
{
    return (int32_t)U_imm31_12;
}

int32_t Decoder::Decoding::getJImm()
{
    uint32_t imm10_1 = J_imm10_1;
    uint32_t imm11 = J_imm11 << 10;
    uint32_t imm19_12 = J_imm19_12 << 11;
    uint32_t imm20 = J_imm20 << 19;
    int32_t imm = imm10_1 | imm11 | imm19_12 | imm20;
    return imm;
}

int32_t Decoder::Decoding::getImm(Type type)
{
    switch(type)
    {
        case I:
            return getIImm();
        case S:
            return getSImm();
        case B:
            return getBImm();
        case U:
            return getUImm();
        case J:
            return getJImm();
        default:
            return 0;
    }
}


Decoder::Decoding Decoder::getDecoding(uint32_t encoding)
{
    Decoding decoding = {
        .opcode     = this->applyMask(encoding, Mask::OPCODE),
        .rd         = this->applyMask(encoding, Mask::RD),
        .funct3     = this->applyMask(encoding, Mask::FUNCT3),
        .rs1        = this->applyMask(encoding, Mask::RS1),
        .rs2        = this->applyMask(encoding, Mask::RS2),
        .funct7     = this->applyMask(encoding, Mask::FUNCT7),
        .I_imm11_0  = this->applyMask(encoding, Mask::I_IMM11_0),
        .S_imm11_5  = this->applyMask(encoding, Mask::S_IMM11_5),
        .S_imm4_0   = this->applyMask(encoding, Mask::S_IMM4_0),
        .B_imm12    = this->applyMask(encoding, Mask::B_IMM12),
        .B_imm10_5  = this->applyMask(encoding, Mask::B_IMM10_5),
        .B_imm4_1   = this->applyMask(encoding, Mask::B_IMM4_1),
        .B_imm11    = this->applyMask(encoding, Mask::B_IMM11),
        .U_imm31_12 = this->applyMask(encoding, Mask::U_IMM31_12),
        .J_imm20    = this->applyMask(encoding, Mask::J_IMM20),
        .J_imm10_1  = this->applyMask(encoding, Mask::J_IMM10_1),
        .J_imm11    = this->applyMask(encoding, Mask::J_IMM11),
        .J_imm19_12 = this->applyMask(encoding, Mask::J_IMM19_12)
    };

    return decoding;
}


Instruction Decoder::decode(uint32_t encoding)
{
    Decoding decoding = getDecoding(encoding);
    Type type = instrType[decoding.opcode];
    auto imm = decoding.getImm(type);

    uint32_t instrKey = 0;
    switch(decoding.opcode)
    {
        case OP:
            instrKey = decoding.funct3 | (decoding.funct7 << 3);
            break;

        case SYSTEM:
            instrKey = decoding.getImm(type);
            break;

        case LUI:
        case AUIPC:
        case JAL:
        case JALR:
            instrKey = 0;
            break;

        case OP_IMM:
            if(decoding.funct3 == SR)
            {
                if(decoding.funct7 == 0)
                    instrKey = decoding.funct3 << 1;
                else
                    instrKey = (decoding.funct3 << 1) + 1;
            }
            else
                instrKey = decoding.funct3;
            break;

        default:
            instrKey = decoding.funct3;
            break;
    }
        
    auto executor = instructions.at(decoding.opcode).at(instrKey);
    
    Instruction instr(  encoding, executor, imm, decoding.rs1,
                        decoding.rs2, decoding.rd);
    return instr;
}



