#pragma once
#include "Coder.h"
#include "HammingCorrector.h"

class HammingDecoder : public Coder {
public:
    explicit HammingDecoder(size_t control_bits);

    bool Code(std::deque<bool>& total, std::deque<bool>& data) override;

private:
    size_t control_bits_;
    size_t total_size_;
    size_t data_size_;

    HammingCorrector hamming_corrector_;
};