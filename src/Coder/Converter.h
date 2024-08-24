#pragma once

#include "Coder.h"
#include <string>
#include <memory>
#include <optional>
#include <deque>
#include <concepts>


class Converter {
public:

    explicit Converter(std::unique_ptr<Coder> coder);

    void SetCoder(std::unique_ptr<Coder> coder);

    std::optional<std::string> TryConvert(const char* str, uint32_t size);

    template<std::unsigned_integral T>
    std::optional<std::string> TryConvert(T t) {
        constexpr T t_max_pow2 = static_cast<T>(-1) - static_cast<T>(-1) / 2 ;

        for (T j = t_max_pow2; j > 0; j /= 2) {
            data_.push_back(t / j % 2);
        }

        while (coder_->Code(total_, data_));

        if (total_.size() < 8) return {};

        return std::move(BitsToString());
    }

    std::string GetRemainder();

private:
    std::optional<std::string> BitsToString();

    std::unique_ptr<Coder> coder_;
    std::deque<bool> data_;
    std::deque<bool> total_;
};