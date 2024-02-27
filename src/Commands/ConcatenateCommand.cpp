#include "ConcatenateCommand.h"
#include <string>

ConcatenateCommand::ConcatenateCommand(
            std::string firstArchiveName,
            std::string secondArchiveName,
            std::string resultArchiveName) :
            firstArchiveName_(std::move(firstArchiveName)),
            secondArchiveName_(std::move(secondArchiveName)),
            resultArchiveName_(std::move(resultArchiveName)) {}

void ConcatenateCommand::Execute() {
    std::cout << "Concatenating " << firstArchiveName_ << " and " << secondArchiveName_ << " into " << resultArchiveName_ << std::endl;
}