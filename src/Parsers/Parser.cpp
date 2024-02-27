#include "Parser.h"

CommandBuilder* Parser::Parse(std::vector<std::string> request) {
    if (optionParser_ == nullptr) {
        return nullptr;
    }
    auto* builder = new CommandBuilder();
    while (!request.empty()) {
        if (!optionParser_->Parse(request, builder)) {
            break;
        }
    }
    if (!request.empty()) return nullptr;
    return builder;
}

ChainLinkParser* Parser::AddOption(ChainLinkParser *parser) {
    if (optionParser_ == nullptr) {
        optionParser_ = parser;
        return parser;
    }
    return optionParser_->AddNextParser(parser);
}


