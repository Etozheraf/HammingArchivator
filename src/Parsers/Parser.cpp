#include "Parser.h"

std::unique_ptr<CommandBuilder> Parser::Parse(std::vector<std::string> request) {
    if (option_parser_ == nullptr) {
        return nullptr;
    }

    std::unique_ptr<CommandBuilder> builder = std::make_unique<CommandBuilder>();
    auto copy(builder.get());

    while (!request.empty()) {
        if (!option_parser_->Parse(request, builder)) {
            break;
        }
    }

    if (copy == builder.get()) {
        return nullptr;
    }

    return std::move(builder);
}

ChainLinkParser& Parser::AddOption(std::unique_ptr<ChainLinkParser> parser) {
    if (option_parser_ == nullptr) {
        option_parser_ = std::move(parser);
        return *option_parser_;
    }
    return option_parser_->AddNextParser(std::move(parser));
}


