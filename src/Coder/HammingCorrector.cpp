#include "HammingCorrector.h"
#include<cmath>

HammingCorrector::HammingCorrector(uint32_t total_size) : total_size_(total_size) { }

void HammingCorrector::Correction(std::deque<bool>& total) const {
    uint32_t index_of_error = 0;

    for (uint32_t i = 0; i < log2(total_size_ + 1); ++i) {
        uint32_t control_bit = (1 << i) - 1;
        uint32_t step = (1 << i);
        bool res = false;
        for (uint32_t j = control_bit; j < total_size_; ++j) {
            res = total[j] xor res;
            if ((j + 1) % step == step - 1) {
                j += step;
            }
        }
        if (!res) continue;
        index_of_error += control_bit + 1;
    }
    if (index_of_error == 0) return;
    total[index_of_error - 1] = !total[index_of_error - 1];
}