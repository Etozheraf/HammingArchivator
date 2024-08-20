#pragma once
#include "Coder.h"

class HammingCoder : public Coder {
public:
    explicit HammingCoder(size_t control_bits);

    bool Code(std::deque<bool>& total, std::deque<bool>& data) override;

private:
    size_t total_size_;
    size_t data_size_;
    size_t control_bits_;
};