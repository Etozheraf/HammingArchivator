#include "ExtractCommand.h"

ExtractCommand::ExtractCommand(std::string archiveName, std::vector<std::string> fileNames)  :
        archiveName_(std::move(archiveName)),
        fileNames_(std::move(fileNames)) {}

void ExtractCommand::Execute() {
    std::cout << "Extracting files from archive " << archiveName_ << ": ";
    for (const auto& fileName : fileNames_) {
        std::cout << fileName << ", ";
    }
}
