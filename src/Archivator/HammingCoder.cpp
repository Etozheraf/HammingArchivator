#include "HammingCoder.h"

HammingCoder::HammingCoder(size_t control_bits) :
        control_bits_(control_bits) {
    total_size_ = (1 << control_bits_) - 1;
    data_size_ = total_size_ - control_bits_;
}

bool HammingCoder::Code(std::deque<bool>& total, std::deque<bool>& data) {
    if (data.size() < data_size_) return false;

    bool t[total_size_];
    for (uint32_t i = 0; i < total_size_; ++i) {
        if ((log2(i + 1) - uint32_t(log2(i + 1)) == 0.0)) {
            t[i] = false;
        } else {
            t[i] = data.front();
            data.pop_front();
        }
    }

    for (uint32_t i = 0; i < control_bits_; ++i) {
        for (uint32_t j = (1 << i); j < total_size_; j++) {
            if (((j + 1) / (1 << i)) % 2 == 1) {
                uint32_t control_pos = (1 << i) - 1;
                t[control_pos] = t[control_pos] xor t[j];
            }
        }
    }

    for (uint32_t i = 0; i < total_size_; ++i) {
        total.push_back(t[i]);
    }
    return true;
}