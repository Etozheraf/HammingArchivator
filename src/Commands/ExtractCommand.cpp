#include "ExtractCommand.h"

ExtractCommand::ExtractCommand(std::string archive_name, std::vector<std::string> file_names)  :
        archive_name_(std::move(archive_name)),
        file_names_(std::move(file_names)) {}

std::string ExtractCommand::Execute() {
    std::cout << "Extracting files from archive " << archive_name_ << ": ";
    for (const auto& fileName : file_names_) {
        std::cout << fileName << ", ";
    }
}
