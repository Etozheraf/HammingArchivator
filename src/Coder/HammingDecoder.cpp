#include "HammingDecoder.h"

HammingDecoder::HammingDecoder(size_t control_bits) :
        control_bits_(control_bits),
        total_size_((1 << control_bits_) - 1),
        data_size_(total_size_ - control_bits_),
        hamming_corrector_(total_size_) { }

bool HammingDecoder::Code(std::deque<bool>& total, std::deque<bool>& data) {
    if (data.size() < total_size_) return false;

    hamming_corrector_.Correction(data);

    for (uint32_t i = 0, j = 0; i < total_size_; ++i, ++j) {
        if ((log2(i + 1) - uint32_t(log2(i + 1)) == 0.0)) {
            data.pop_front();
            continue;
        }
        total.push_back(data.front());
        data.pop_front();
    }

    return true;
}
