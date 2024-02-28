#pragma once

#include <vector>
#include <memory>
#include "../Builders/CommandBuilder.hpp"

struct ChainLinkParser {

    virtual bool Parse(std::vector<std::string>& request, std::unique_ptr<CommandBuilder>& builder) = 0;

    virtual ChainLinkParser& AddNextParser(std::unique_ptr<ChainLinkParser> parser) = 0;

    virtual ~ChainLinkParser() = default;
};

class ChainLinkParserBase : public ChainLinkParser {
public:
    ChainLinkParser& AddNextParser(std::unique_ptr<ChainLinkParser> parser) override {
        if (next_parser_ == nullptr) {
            next_parser_ = std::move(parser);
            return *next_parser_;
        }
        return next_parser_->AddNextParser(std::move(parser));
    };

    virtual ~ChainLinkParserBase() = default;

protected:
    std::unique_ptr<ChainLinkParser> next_parser_ = nullptr;
};