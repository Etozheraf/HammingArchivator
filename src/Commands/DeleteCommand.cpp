#include "DeleteCommand.h"

DeleteCommand::DeleteCommand(std::string archiveName, std::vector<std::string> fileNames)  :
        archiveName_(std::move(archiveName)),
        fileNames_(std::move(fileNames)) {}

void DeleteCommand::Execute() {
    std::cout << "Deleting files from archive " << archiveName_ << ": ";
    for (const auto& fileName : fileNames_) {
        std::cout << fileName << ", ";
    }
}
