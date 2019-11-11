
#include "decoder.h"


//Instruction* Decoder::decode(uint32_t encoding)
         
#include <iostream>

int Decoder::findFirstUp(uint32_t raw)
{
    int bit = 0;
    uint32_t mask = 0x1;
    int nbit = sizeof(uint32_t) * BYTE_SIZE;
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

#define GET_FIELD(field, what) decoding.#field = this->applyMask(encoding, masks[what])

Decoder::Decoding Decoder::decode(uint32_t encoding)
{
    Decoding decoding;
    decoding.opcode = this->applyMask(encoding, masks[OPCODE]);
    GET_FIELD(opcode, OPCODE);
}





