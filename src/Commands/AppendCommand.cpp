#include "AppendCommand.h"

AppendCommand::AppendCommand(std::string archiveName, std::vector<std::string> fileNames) :
        archiveName_(std::move(archiveName)),
        fileNames_(std::move(fileNames)) {}

void AppendCommand::Execute() {
    std::cout << "Appending files to archive " << archiveName_ << ": ";
    for (const auto& fileName : fileNames_) {
        std::cout << fileName << ", ";
    }
}
