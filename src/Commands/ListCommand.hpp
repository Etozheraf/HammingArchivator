#pragma once

#include "Command.hpp"
#include <string>

class ListCommand : public Command {
public:
    explicit ListCommand(std::string archiveName);

    void Execute() override;

private:
    std::string archiveName_;
};