#pragma once
#include "Coder.h"
#include "HammingCorrector.h"

class ThreeBitsDecoder : public Coder {
public:
    ThreeBitsDecoder();

    bool Code(std::deque<bool>& total, std::deque<bool>& data) override;
private:
    HammingCorrector hamming_corrector_;
};