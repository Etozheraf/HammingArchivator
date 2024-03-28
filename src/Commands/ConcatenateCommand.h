#pragma once

#include "Command.hpp"
#include <string>

class ConcatenateCommand : public Command {
public:
    explicit ConcatenateCommand(std::string first_archive_name,
                                std::string second_archive_name,
                                std::string result_archive_name);

    std::string Execute() override;

private:
    std::string first_archive_name_;
    std::string second_archive_name_;

    std::string result_archive_name_;
};