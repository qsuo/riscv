#include "log_service.h"

string addrToStr(uint32_t addr)
{
    stringstream ss;
    ss << "0x" << hex << setfill('0') << setw(8) << addr;
    return ss.str();
}

string regToStr(uint8_t num)
{
    return "x" + to_string(num);
}

string immToStr(uint32_t imm)
{
    stringstream ss;
    ss << "0x" << hex << setfill('0') << setw(8) << imm;
    return ss.str();
}
