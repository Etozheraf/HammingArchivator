#pragma once
#include<deque>
#include<cmath>
#include<cstdint>

class Coder {
public:
    virtual bool Code(std::deque<bool>& total, std::deque<bool>& data) = 0;
};