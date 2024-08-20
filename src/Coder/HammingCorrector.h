#include<deque>
#include<cstdint>

class HammingCorrector {
public:
    explicit HammingCorrector(uint32_t total_size);

    void Correction(std::deque<bool>& total) const;

private:
    uint32_t total_size_;
};