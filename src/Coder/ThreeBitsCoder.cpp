#include "ThreeBitsCoder.h"

bool ThreeBitsCoder::Code(std::deque<bool>& total, std::deque<bool>& data) {
    if (data.empty()) return false;

    for (bool i: data) {
        total.push_back(i);
        total.push_back(i);
        total.push_back(i);
    }
    data.clear();
    return true;
}