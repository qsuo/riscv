
#ifndef __CACHE_H__
#define __CACHE_H__

#include <queue>
#include <unoredred_map>

#include "basic_block.h"

class Cache
{
public:
    std::queue<uint32_t> lru;
    std::unordered_map<uint32_t, BasicBlock*> map;


};

#endif
