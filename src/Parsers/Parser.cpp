#include "Parser.h"

std::unique_ptr<CommandBuilder> Parser::Parse(std::vector<std::string> request) {
    if (optionParser_ == nullptr) {
        return nullptr;
    }

    std::unique_ptr<CommandBuilder> builder = std::make_unique<CommandBuilder>();
    auto copy(builder.get());

    while (!request.empty()) {
        if (!optionParser_->Parse(request, builder)) {
            break;
        }
    }

    if (copy == builder.get()) {
        return nullptr;
    }

    return std::move(builder);
}

ChainLinkParser& Parser::AddOption(std::unique_ptr<ChainLinkParser> parser) {
    if (optionParser_ == nullptr) {
        optionParser_ = std::move(parser);
        return *optionParser_;
    }
    return optionParser_->AddNextParser(std::move(parser));
}


