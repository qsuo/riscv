
#include "../memory.h"
#include <cassert>
#include <iostream>

const uint32_t MEMORY_SIZE = 1000;

int main()
{
    Memory memory(MEMORY_SIZE);
    assert(memory.getSize() == MEMORY_SIZE);

    int data = 10;
    memory.write(500, &data, sizeof(data));
    int tmp = 0;
    memory.read(500, &tmp, sizeof(tmp));
    assert(tmp == data);
    return 0;
}
