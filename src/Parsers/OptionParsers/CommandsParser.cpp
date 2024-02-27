#include "CommandsParser.h"

bool CommandsParser::Parse(std::vector<std::string>& request, CommandBuilder*& builder) {
    if (!is_chosen_) {
        std::string parsed_word = request[0];
        ChainLinkParser* argumentParser = commands_->Parse(request, builder);

        if (request.empty()) return true;

        if (parsed_word != request[0]) {
            is_chosen_ = true;
            AddNextParser(argumentParser);
            commands_->Parse(request, builder, true);
            return true;
        }
    }
    if (next_parser_ == nullptr)
        return false;
    return next_parser_->Parse(request, builder);
}

CommandChainLinkParser* CommandsParser::AddCommand(CommandChainLinkParser* parser) {
    if (commands_ == nullptr) {
        commands_ = parser;
        return parser;
    }
    return commands_->AddNextParser(parser);
}
