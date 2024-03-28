#pragma once

#include "CommandBuilder.hpp"

class ConcatenateCommandBuilder : public CommandBuilder {
public:

    explicit ConcatenateCommandBuilder(const CommandBuilder& c) : CommandBuilder(c) {};

    ConcatenateCommandBuilder& SetFirstArchiveName(const std::string& first_archive_name);

    ConcatenateCommandBuilder& SetSecondArchiveName(const std::string& second_archive_name);

    std::string ShowErrors() override;

    Command* TryBuild() override;

private:
    std::string first_archive_name_;
    std::string secondArchiveName_;
};