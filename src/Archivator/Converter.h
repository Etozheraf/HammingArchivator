#pragma once
#include "Coder.h"
#include <string>
#include <memory>
#include <optional>
#include <deque>


class Converter {
public:
    Converter() = default;

    explicit Converter(std::unique_ptr<Coder> coder);

    void SetCoder(std::unique_ptr<Coder> coder);

    template<typename T>
    std::optional<std::string> TryConvert(T t);

    std::string GetRemainder();

private:
    void BitsToString(std::string& res);

    std::unique_ptr<Coder> coder_;
    std::deque<bool> data_;
    std::deque<bool> total_;
};