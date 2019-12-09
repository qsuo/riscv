
#include "simulator.h"
#include <iostream>
#include <stdio.h>

#define SIZE 10000000

int main(int argc, char** argv)
{
    Simulator sim(SIZE);
    sim.load(argv[1]);
    sim.run();
    return 0;
}
