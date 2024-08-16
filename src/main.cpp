#include "Commands/CreateCommand.h"
#include "Commands/AppendCommand.h"
#include "Commands/ExtractCommand.h"
#include "Commands/DeleteCommand.h"
#include "Parsers/Parser.h"
#include "Parsers/OptionParsers/CommandParser.h"
#include "Parsers/OptionParsers/CommandsParser.h"
#include "Parsers/OptionParsers/ArchiveNameParser.h"
#include "Parsers/ArgumentParsers/FileNameParser.h"
#include "Builders/CommandBuilder.hpp"
#include "Builders/CommandWithFileNameBuilder.h"
#include "Builders/ListCommandBuilder.h"
#include "Builders/CreateCommandBuilder.h"
#include "Parsers/ArgumentParsers/ControlBitsParser.h"
#include <iostream>
#include <memory>

/*
header:
HAF 3
control bits 4
offset1 4
filename1 n1
offset2 4
filename2 n2
...
file_size 4
 */

int main(int argc, char* argv[]) {
    Parser parser;
    std::unique_ptr<CommandsParser> commands_parser = std::make_unique<CommandsParser>();

    auto create_argument_parsers = std::make_unique<ControlBitsParser>();
    create_argument_parsers->AddNextParser(std::make_unique<FileNameParser<CommandWithFileNameBuilder<CreateCommand>>>());

    commands_parser
            ->AddCommand(std::make_unique<CommandParser<CreateCommandBuilder>>(
                    std::move(create_argument_parsers),
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
                    'd'));

    parser.AddOption(std::make_unique<ArchiveNameParser>())
          .AddNextParser(std::move(commands_parser));

    if (argc == 1) {
        std::cout << "No arguments provided" << std::endl;
        return 0;
    }

    std::unique_ptr<CommandBuilder> command_builder = std::move(
            parser.Parse(std::vector<std::string>(argv + 1, argv + argc)));

    if (command_builder == nullptr) {
        std::cout << "Invalid arguments" << std::endl;
        return 0;
    }

    std::string errors = command_builder->ShowErrors();
    if (!errors.empty()) {
        std::cout << errors << std::endl;
        return 0;
    }

    errors = command_builder->TryBuild()->Execute();
    if (!errors.empty()) {
        std::cout << errors << '\n';
    }
    return 0;
}
