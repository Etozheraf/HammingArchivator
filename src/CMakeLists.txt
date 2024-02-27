add_executable(HamArc main.cpp
       Commands/ListCommand.cpp
       Commands/ConcatenateCommand.cpp
       Parsers/Parser.cpp
       Parsers/OptionParsers/ArchiveNameParser.cpp
       Commands/ExtractCommand.cpp
       Commands/DeleteCommand.cpp
       Commands/CreateCommand.cpp
       Commands/CreateCommand.h
       Commands/AppendCommand.cpp
       Commands/AppendCommand.h
       Builders/ListCommandBuilder.cpp
       Builders/ListCommandBuilder.h
       Builders/ConcatenateCommandBuilder.cpp
       Builders/ConcatenateCommandBuilder.h
       Builders/CommandWithFileNameBuilder.h
       Parsers/ArgumentParsers/FileNameParser.h
       Parsers/ArgumentParsers/MergedArchivesNameParser.cpp
       Parsers/ArgumentParsers/MergedArchivesNameParser.h
       Parsers/OptionParsers/CommandsParser.cpp
       Parsers/OptionParsers/CommandsParser.h
)

#target_link_libraries(HamArc Operations)

#target_include_directories(HamArc PUBLIC ${PROJECT_SOURCE_DIR})