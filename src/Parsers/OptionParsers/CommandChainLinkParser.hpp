#pragma once

#include <vector>
#include "../ChainLinkParser.hpp"

struct CommandChainLinkParser {

    virtual std::unique_ptr<ChainLinkParser> Parse(std::vector<std::string>& request, std::unique_ptr<CommandBuilder>& builder, bool is_command_created) = 0;

    virtual CommandChainLinkParser& AddNextParser(std::unique_ptr<CommandChainLinkParser> parser) = 0;
};

class CommandChainLinkParserBase : public CommandChainLinkParser {
public:
    CommandChainLinkParser& AddNextParser(std::unique_ptr<CommandChainLinkParser> parser) override {
        if (next_parser_ == nullptr) {
            next_parser_ = std::move(parser);
            return *next_parser_;
        }
        return next_parser_->AddNextParser(std::move(parser));
    };

    virtual ~CommandChainLinkParserBase() = default;

protected:
    std::unique_ptr<CommandChainLinkParser> next_parser_ = nullptr;
};