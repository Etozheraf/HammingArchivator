#pragma once

#include "Command.hpp"
#include <vector>
#include <string>

class CreateCommand : public Command {
public:
    explicit CreateCommand(std::string archiveName, std::vector<std::string> fileNames);

    void Execute() override;

private:
    std::string archiveName_;
    std::vector<std::string> fileNames_;
};