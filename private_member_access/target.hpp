#pragma once

#include <cstdint>

class Target {
public:
    int32_t getX() { return x_; }
private:
    // hey, private data member, you think we can't access you directly?
    // We'll see.
    int32_t x_ {0};    
};