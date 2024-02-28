#include "Commands/CreateCommand.h"
#include "Commands/AppendCommand.h"
#include "Commands/ExtractCommand.h"
#include "Commands/DeleteCommand.h"
#include "Parsers/Parser.h"
#include "Parsers/OptionParsers/CommandParser.h"
#include "Parsers/OptionParsers/CommandsParser.h"
#include "Parsers/OptionParsers/ArchiveNameParser.h"
#include "Parsers/ArgumentParsers/FileNameParser.h"
#include "Parsers/ArgumentParsers/MergedArchivesNameParser.h"
#include "Builders/CommandBuilder.hpp"
#include "Builders/CommandWithFileNameBuilder.h"
#include "Builders/ConcatenateCommandBuilder.h"
#include "Builders/ListCommandBuilder.h"
#include <iostream>
#include <memory>

/*
header:
HAF 3
control bits 7
offset1 11
filename1 22 17
offset2 26 21
filename2 32
offset3 36
...
 */

int main(int argc, char* argv[]) {
    Parser parser;
    std::unique_ptr<CommandsParser> commandsParser = std::make_unique<CommandsParser>();

    commandsParser
            ->AddCommand(std::make_unique<CommandParser<CommandWithFileNameBuilder<CreateCommand>>>(
                    std::make_unique<FileNameParser<CommandWithFileNameBuilder<CreateCommand>>>(),
                    "create",
                    'c'))
            .AddNextParser(std::make_unique<CommandParser<ListCommandBuilder>>(
                    "list",
                    'l'))
            .AddNextParser(std::make_unique<CommandParser<CommandWithFileNameBuilder<ExtractCommand>>>(
                    std::make_unique<FileNameParser<CommandWithFileNameBuilder<ExtractCommand>>>(),
                    "extract",
                    'x'))
            .AddNextParser(std::make_unique<CommandParser<CommandWithFileNameBuilder<AppendCommand>>>(
                    std::make_unique<FileNameParser<CommandWithFileNameBuilder<AppendCommand>>>(),
                    "append",
                    'a'))
            .AddNextParser(std::make_unique<CommandParser<CommandWithFileNameBuilder<DeleteCommand>>>(
                    std::make_unique<FileNameParser<CommandWithFileNameBuilder<DeleteCommand>>>(),
                    "delete",
                    'd'))
            .AddNextParser(std::make_unique<CommandParser<ConcatenateCommandBuilder>>(
                    std::make_unique<MergedArchivesNameParser>(),
                    "concatenate",
                    'A'));

    parser
            .AddOption(std::make_unique<ArchiveNameParser>())
            .AddNextParser(std::move(commandsParser));

    if (argc == 1) {
        std::cout << "No arguments provided" << std::endl;
        return 0;
    }

    std::unique_ptr<CommandBuilder> commandBuilder = std::move(parser.Parse(std::vector<std::string>(argv + 1, argv + argc)));

    if (commandBuilder == nullptr) {
        std::cout << "Invalid arguments" << std::endl;
        return 0;
    }

    std::string errors = commandBuilder->ShowErrors();
    if (!errors.empty()) {
        std::cout << errors << std::endl;
        return 0;
    }

    commandBuilder->TryBuild()->Execute();
    return 0;
}
