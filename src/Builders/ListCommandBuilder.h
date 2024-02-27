#pragma once

#include "CommandBuilder.hpp"

class ListCommandBuilder : public CommandBuilder {
public:

    ListCommandBuilder() = default;

    explicit ListCommandBuilder(const CommandBuilder& c) : CommandBuilder(c) {};

    std::string ShowErrors() override;

    Command* TryBuild() override;
};