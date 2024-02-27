#pragma once

#include "CommandBuilder.hpp"

class ConcatenateCommandBuilder : public CommandBuilder {
public:

    explicit ConcatenateCommandBuilder(const CommandBuilder& c) : CommandBuilder(c) {};

    ConcatenateCommandBuilder& SetFirstArchiveName(const std::string& firstArchiveName);

    ConcatenateCommandBuilder& SetSecondArchiveName(const std::string& secondArchiveName);

    std::string ShowErrors() override;

    Command* TryBuild() override;

private:
    std::string firstArchiveName_;
    std::string secondArchiveName_;
};