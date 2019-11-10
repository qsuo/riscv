
#ifndef __SIMULATOR_H__
#define __SIMULATOR_H__

#include "memory.h"

class Simulator
{
public:
    Simulator(std::size_t memorySize);
    void load(const char* filePath);
    void run();

    std::size_t getMemorySize()
    {
        return memory_.getSize();
    }

private:
    Memory memory_;

};

#endif
