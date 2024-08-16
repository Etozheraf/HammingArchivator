#pragma once
#include "CommandWithFileNameBuilder.h"
#include "../Commands/CreateCommand.h"

class CreateCommandBuilder : public CommandWithFileNameBuilder<CreateCommand> {
public:

    explicit CreateCommandBuilder(const CommandBuilder& c) : CommandWithFileNameBuilder<CreateCommand>(c) {};

    CreateCommandBuilder& SetControlBits(int control_bits);

    std::string ShowErrors() override;

    CreateCommand* TryBuild() override;

private:
    int control_bits_ = 0;
};