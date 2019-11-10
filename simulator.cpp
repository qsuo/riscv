
#include <iostream>

#include <unistd.h>
#include <fcntl.h>

#include "simulator.h"

Simulator::Simulator(std::size_t memorySize):
    memory_(memorySize)
{}

void Simulator::load(const char* filePath)
{
    int fd = open(filePath, O_RDONLY);
    //read(fd, 
    close(fd);
}

void Simulator::run()
{
    //
}


