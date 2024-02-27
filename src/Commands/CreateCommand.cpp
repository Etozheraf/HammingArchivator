#include "CreateCommand.h"

CreateCommand::CreateCommand(std::string archiveName, std::vector<std::string> fileNames) :
        archiveName_(std::move(archiveName)),
        fileNames_(std::move(fileNames)) {}

void CreateCommand::Execute() {
    std::cout << "Creating archive " << archiveName_ << " with files: ";
    for (const auto& fileName : fileNames_) {
        std::cout << fileName << ", ";
    }
}
