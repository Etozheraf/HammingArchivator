#include "ListCommand.hpp"
#include <string>

ListCommand::ListCommand(std::string archiveName) : archiveName_(std::move(archiveName)) {}

void ListCommand::Execute() {
    std::cout << "Listing files from archive " << archiveName_ << ": ";
}
