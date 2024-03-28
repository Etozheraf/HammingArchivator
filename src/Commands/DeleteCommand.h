#pragma once

#include "Command.hpp"
#include <vector>
#include <string>

class DeleteCommand : public Command {
public:
    explicit DeleteCommand(std::string archive_name, std::vector<std::string> file_names);

    std::string Execute() override;

private:
    std::string archive_name_;
    std::vector<std::string> file_names_;
};