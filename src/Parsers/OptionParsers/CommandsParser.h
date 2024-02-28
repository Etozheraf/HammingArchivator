#pragma once

#include "../ChainLinkParser.hpp"
#include "CommandChainLinkParser.hpp"


class CommandsParser : public ChainLinkParserBase {
public:

    bool Parse(std::vector<std::string>& request, std::unique_ptr<CommandBuilder>& builder) override;

    CommandChainLinkParser& AddCommand(std::unique_ptr<CommandChainLinkParser> parser);

private:
    bool is_chosen_ = false;
    std::unique_ptr<CommandChainLinkParser> commands_ = nullptr;
};
