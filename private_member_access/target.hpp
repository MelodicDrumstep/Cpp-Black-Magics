#pragma once

#include <cstdint>

class Target {
public:
    int32_t getX() { return x_; }
private:
    int32_t x_ {0};    
};