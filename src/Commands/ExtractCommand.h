#pragma once

#include "Command.hpp"
#include <vector>
#include <string>

class ExtractCommand : public Command {
public:
    explicit ExtractCommand(std::string archive_name, std::vector<std::string> file_names);

    std::string Execute() override;

private:
    std::string archive_name_;
    std::vector<std::string> extracted_filenames_;
};