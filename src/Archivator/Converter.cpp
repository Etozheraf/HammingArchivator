#include "Converter.h"


Converter::Converter(std::unique_ptr<Coder> coder) : coder_(std::move(coder)) {}

void Converter::SetCoder(std::unique_ptr<Coder> coder) {
    coder_ = std::move(coder);
}

template<typename T>
std::optional<std::string> Converter::TryConvert(T t) {
    const int t_size = 8 * sizeof(t) - 1;
    const int t_pow_2 = 1 >> t_size;

    for (int j = t_pow_2; j > 0; j >>= 1) {
        data_.push_back((t & j) / j);
    }

    while (coder_->Code(total_, data_));

    if (total_.size() < 8) return {};

    std::string result;
    BitsToString(result);
    return std::move(result);
}

std::string Converter::GetRemainder() {
    while (!data_.empty()) {
        data_.push_back(false);
        coder_->Code(total_, data_);
    }

    while (total_.size() % 8) {total_.push_back(false);}

    std::string result;
    BitsToString(result);
    return std::move(result);
}

void Converter::BitsToString(std::string& res) {
    while (total_.size() > 7) {
        uint8_t buf = 0;
        for (int i = 0; i < 8; ++i) {
            buf |= (total_.front() * 0b10000000);
            buf >>= 1;
            total_.pop_front();
        }
        res.push_back(static_cast<char>(buf));
    }
}
