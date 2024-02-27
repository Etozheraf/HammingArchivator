#pragma once

#include "Command.hpp"
#include <string>

class ConcatenateCommand : public Command {
public:
    explicit ConcatenateCommand(std::string firstArchiveName,
                                std::string secondArchiveName,
                                std::string resultArchiveName);

    void Execute() override;

private:
    std::string firstArchiveName_;
    std::string secondArchiveName_;

    std::string resultArchiveName_;
};