#include "ThreeBitsDecoder.h"

ThreeBitsDecoder::ThreeBitsDecoder() : hamming_corrector_(3) { }

bool ThreeBitsDecoder::Code(std::deque<bool>& total, std::deque<bool>& data) {
    if (data.size() < 3) return false;

    hamming_corrector_.Correction(data);
    total.push_back(data.front());
    for (int i = 0; i < 3; ++i)
        data.pop_front();

    return true;
}