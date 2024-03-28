#pragma once

#include "Command.hpp"
#include <string>

class ListCommand : public Command {
public:
    explicit ListCommand(std::string archive_name);

    std::string Execute() override;

private:
    std::string archive_name_;
};