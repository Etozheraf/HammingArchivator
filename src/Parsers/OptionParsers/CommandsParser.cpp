#include "CommandsParser.h"

bool CommandsParser::Parse(std::vector<std::string>& request, std::unique_ptr<CommandBuilder>& builder) {
    if (!is_chosen_) {
        std::string parsed_word = request[0];
        std::unique_ptr<ChainLinkParser> argumentParser = std::move(commands_->Parse(request, builder, false));

        if (request.empty()) return true;

        if (parsed_word != request[0]) {
            is_chosen_ = true;
            AddNextParser(std::move(argumentParser));
            commands_->Parse(request, builder, true);
            return true;
        }
    }
    if (next_parser_ == nullptr)
        return false;
    return next_parser_->Parse(request, builder);
}

CommandChainLinkParser& CommandsParser::AddCommand(std::unique_ptr<CommandChainLinkParser> parser) {
    if (commands_ == nullptr) {
        commands_ = std::move(parser);
        return *commands_;
    }
    return commands_->AddNextParser(std::move(parser));
}
