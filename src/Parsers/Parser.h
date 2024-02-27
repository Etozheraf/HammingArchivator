#pragma once

#include "ChainLinkParser.hpp"

class Parser{
public:
    CommandBuilder* Parse(std::vector<std::string> request);

    ChainLinkParser* AddOption(ChainLinkParser* parser);

private:
    ChainLinkParser* optionParser_ = nullptr;
};