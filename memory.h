
#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <cstddef>
#include <cstdint>

typedef uint32_t address_t;

class Memory
{
public:
    Memory(size_t size);
    ~Memory();
    void read(address_t address, void* dst, std::size_t nbyte);
    void write(address_t address, void* src, std::size_t nbyte);

    std::size_t getSize()
    {
        return size_;
    }
    //int dump(void* start=)

private:
    std::size_t size_;
    uint8_t* memory_; 
};

#endif

