
#ifndef __INSTRUCTION_H__
#define __INSTRUCTION_H__

#include <stdint.h>

typedef int(*executor)(Instruction& this, Instruction& next)

class Instrusction()
{
public:
    Instruction(uint32_t encoding);
    ~Incstruction();
    executor execute;
    
private:
    int decode(uint32_t encoding);

    uint32_t m_encoding;

    uint16_t m_imm;
    uint8_t m_rs1;
    uint8_t m_rs2;
    uint8_t m_rd;
}

#endif 
