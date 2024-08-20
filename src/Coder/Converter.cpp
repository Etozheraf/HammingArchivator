#include "Converter.h"

Converter::Converter(std::unique_ptr<Coder> coder) : coder_(std::move(coder)) {}

void Converter::SetCoder(std::unique_ptr<Coder> coder) {
    coder_ = std::move(coder);
}

//template<typename T>
//std::optional<std::string> Converter::TryConvert(T t) {
//    return TryConvert(static_cast<char*>(&t), sizeof(t));
//}

std::string Converter::GetRemainder() {
    while (!data_.empty()) {
        data_.push_back(false);
        coder_->Code(total_, data_);
    }

    while (total_.size() % 8) { total_.push_back(false); }

    std::string result;
    BitsToString(result);
    return std::move(result);
}

void Converter::BitsToString(std::string& res) {
    while (total_.size() > 7) {
        uint8_t buf = 0;
        for (int i = 0; i < 8; ++i) {
            buf <<= 1;
            buf |= total_.front();
            total_.pop_front();
        }
        res.push_back(static_cast<char>(buf));
    }
}

std::optional<std::string> Converter::TryConvert(const char* str, uint32_t size) {
    const int max_bit = 0b10000000;

    for (int i = 0; i < size; ++i) {
        for (int bit = max_bit; bit > 0; bit >>= 1) {
            data_.push_back((str[i] & bit) / bit);
        }
    }

    while (coder_->Code(total_, data_));

    if (total_.size() < 8) return {};

    std::string result;
    result.reserve(total_.size() / 8);
    BitsToString(result);
    return std::move(result);
}
