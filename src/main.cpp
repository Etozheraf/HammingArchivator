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
   auto* commandsParser = new CommandsParser;

   commandsParser
           ->AddCommand(new CommandParser<CommandWithFileNameBuilder<CreateCommand>>(
                   new FileNameParser<CommandWithFileNameBuilder<CreateCommand>>(),
                   "create",
                   'c'))
           ->AddNextParser(new CommandParser<ListCommandBuilder>(
                   "list",
                   'l'))
           ->AddNextParser(new CommandParser<CommandWithFileNameBuilder<ExtractCommand>>(
                   new FileNameParser<CommandWithFileNameBuilder<ExtractCommand>>(),
                   "extract",
                   'x'))
           ->AddNextParser(new CommandParser<CommandWithFileNameBuilder<AppendCommand>>(
                   new FileNameParser<CommandWithFileNameBuilder<AppendCommand>>(),
                   "append",
                   'a'))
           ->AddNextParser(new CommandParser<CommandWithFileNameBuilder<DeleteCommand>>(
                   new FileNameParser<CommandWithFileNameBuilder<DeleteCommand>>(),
                   "delete",
                   'd'))
           ->AddNextParser(new CommandParser<ConcatenateCommandBuilder>(
                   new MergedArchivesNameParser(),
                   "concatenate",
                   'A'));

   parser.AddOption(new ArchiveNameParser)
           ->AddNextParser(commandsParser);

   if (argc == 1) {
       std::cout << "No arguments provided" << std::endl;
       return 0;
   }

   CommandBuilder* commandBuilder = parser.Parse(std::vector<std::string>(argv + 1, argv + argc));

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
