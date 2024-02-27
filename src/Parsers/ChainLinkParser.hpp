#pragma once

#include <vector>
#include "../Builders/CommandBuilder.hpp"

struct ChainLinkParser {

    virtual bool Parse(std::vector<std::string>& request, CommandBuilder*& builder) = 0;

    virtual ChainLinkParser* AddNextParser(ChainLinkParser* parser) = 0;
};

class ChainLinkParserBase : public ChainLinkParser {
public:
    ChainLinkParser* AddNextParser(ChainLinkParser* parser) override {
        if (next_parser_ == nullptr) {
            next_parser_ = parser;
            return parser;
        }
        return next_parser_->AddNextParser(parser);
    };

    virtual ~ChainLinkParserBase() = default;

protected:
    ChainLinkParser* next_parser_ = nullptr;
};