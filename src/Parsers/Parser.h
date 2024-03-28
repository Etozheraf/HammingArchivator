#pragma once

#include "ChainLinkParser.hpp"
#include <memory>

class Parser{
public:
    std::unique_ptr<CommandBuilder> Parse(std::vector<std::string> request);

    ChainLinkParser& AddOption(std::unique_ptr<ChainLinkParser> parser);

private:
    std::unique_ptr<ChainLinkParser> option_parser_ = nullptr;
};