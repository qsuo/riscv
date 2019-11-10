
#ifndef __DECODER_H__
#define __DECODER_H__

#include <stdint.h>

#include "instruction.h"

class Decoder
{
public:
    Decoder();
    ~Decoder();

    Instruction* decode(uint32_t encoding);
    
private:
}

#endif
