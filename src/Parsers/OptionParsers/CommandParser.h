#pragma once

#include "CommandChainLinkParser.hpp"

template<DerivedFromCommandBuilder Builder>
class CommandParser : public CommandChainLinkParserBase {
public:
    explicit CommandParser(std::string command, char short_command) :
            argument_parser_(nullptr), command_(std::move(command)), short_command_(short_command) {}

    explicit CommandParser(std::unique_ptr<ChainLinkParser> argument_parser, std::string command, char short_command) :
            argument_parser_(std::move(argument_parser)),
            command_(std::move(command)),
            short_command_(short_command) {}

    std::unique_ptr<ChainLinkParser>
    Parse(std::vector<std::string>& request, std::unique_ptr<CommandBuilder>& builder, bool is_command_created) override {
        bool is_command = false;
        std::string short_command = "-";
        short_command += short_command_;

        if (request[0] == ("--" + command_)) {
            is_command = true;
            request.erase(request.begin());
        } else if (request[0].size() >= 2 && request[0].starts_with(short_command)) {
            is_command = true;
            request[0].erase(request[0].begin() + 1);
            if (request[0] == "-")
                request.erase(request.begin());
        }

        if (is_command && !is_command_created) {
            builder = std::make_unique<Builder>(*builder);
            is_command_created = true;
        }

        std::unique_ptr<ChainLinkParser> argument_parser = nullptr;
        if (next_parser_ != nullptr)
            argument_parser = next_parser_->Parse(request, builder, is_command_created);

        if (is_command_created)
            return std::move(argument_parser_);
        return std::move(argument_parser);
    }

private:
    std::unique_ptr<ChainLinkParser> argument_parser_;
    std::string command_;
    char short_command_;
};
