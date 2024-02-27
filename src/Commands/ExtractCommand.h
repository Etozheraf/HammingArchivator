#pragma once

#include "Command.hpp"
#include <vector>
#include <string>

class ExtractCommand : public Command {
public:
    explicit ExtractCommand(std::string archiveName, std::vector<std::string> fileNames);

    void Execute() override;

private:
    std::string archiveName_;
    std::vector<std::string> fileNames_;
};