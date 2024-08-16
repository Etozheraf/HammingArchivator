#pragma once
#include "Coder.h"

class ThreeBitsCoder : public Coder {
public:
    bool Code(std::deque<bool>& total, std::deque<bool>& data) override;
};