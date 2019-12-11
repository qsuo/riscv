
#include "simulator.h"
#include <iostream>
#include <stdio.h>

#define SIZE 10000000

int main(int argc, char** argv)
{
    Simulator sim(SIZE);
    sim.load(argv[1]);
    //sim.run();
    
    int count = 0;
    BasicBlock* bb = NULL;
    while(true)
    {
        bb = sim.getBasicBlock();
        if(bb == NULL)
            break;
        count += bb->count;
        sim.execute(bb);
    }
    std::cout << count << "\n";
    return 0;
}
