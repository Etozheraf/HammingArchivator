#pragma once

#include "../ChainLinkParser.hpp"
#include "CommandChainLinkParser.hpp"


class CommandsParser : public ChainLinkParserBase {
public:

    bool Parse(std::vector<std::string>& request, CommandBuilder*& builder) override;

    CommandChainLinkParser* AddCommand(CommandChainLinkParser* parser);

private:
    bool is_chosen_ = false;
    CommandChainLinkParser* commands_ = nullptr;
};
