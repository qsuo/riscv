
#include "../decoder.h"
#include <iostream>
#include <cassert>

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

    return 0;
}
