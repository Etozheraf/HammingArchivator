add_executable(HamArc main.cpp
        Commands/ListCommand.cpp
        Parsers/Parser.cpp
        Parsers/OptionParsers/ArchiveNameParser.cpp
        Commands/ExtractCommand.cpp
        Commands/DeleteCommand.cpp
        Commands/CreateCommand.cpp
        Commands/AppendCommand.cpp
        Builders/ListCommandBuilder.cpp
        Parsers/OptionParsers/CommandsParser.cpp
        Parsers/ArgumentParsers/ControlBitsParser.cpp
        Builders/CreateCommandBuilder.cpp
        Archivator/Converter.cpp
)

#target_link_libraries(HamArc Operations)

#target_include_directories(HamArc PUBLIC ${PROJECT_SOURCE_DIR})