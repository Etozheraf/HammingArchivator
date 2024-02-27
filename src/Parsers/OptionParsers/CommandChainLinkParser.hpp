#pragma once

#include <vector>
#include "../ChainLinkParser.hpp"

struct CommandChainLinkParser {

    virtual ChainLinkParser* Parse(std::vector<std::string>& request, CommandBuilder*& builder, bool is_command_created = false) = 0;

    virtual CommandChainLinkParser* AddNextParser(CommandChainLinkParser* parser) = 0;
};

class CommandChainLinkParserBase : public CommandChainLinkParser {
public:
    CommandChainLinkParser* AddNextParser(CommandChainLinkParser* parser) override {
        if (next_parser_ == nullptr) {
            next_parser_ = parser;
            return parser;
        }
        return next_parser_->AddNextParser(parser);
    };

    virtual ~CommandChainLinkParserBase() = default;

protected:
    CommandChainLinkParser* next_parser_ = nullptr;
};