#include "DeleteCommand.h"

DeleteCommand::DeleteCommand(std::string archive_name, std::vector<std::string> file_names)  :
        archive_name_(std::move(archive_name)),
        file_names_(std::move(file_names)) {}

std::string DeleteCommand::Execute() {
    std::cout << "Deleting files from archive " << archive_name_ << ": ";
    for (const auto& fileName : file_names_) {
        std::cout << fileName << ", ";
    }
}
