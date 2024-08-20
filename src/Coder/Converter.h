#pragma once
#include "Coder.h"
#include <string>
#include <memory>
#include <optional>
#include <deque>


class Converter {
public:

    explicit Converter(std::unique_ptr<Coder> coder);

    void SetCoder(std::unique_ptr<Coder> coder);

    std::optional<std::string> TryConvert(const char* str, uint32_t size);

    template<typename T>
    std::optional<std::string> TryConvert(T t) {
        return TryConvert(reinterpret_cast<char*>(&t), sizeof(t));
    }

    std::string GetRemainder();

private:
    void BitsToString(std::string& res);

    std::unique_ptr<Coder> coder_;
    std::deque<bool> data_;
    std::deque<bool> total_;
};