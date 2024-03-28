#include "ListCommand.hpp"
#include <string>

ListCommand::ListCommand(std::string archive_name) : archive_name_(std::move(archive_name)) {}

std::string ListCommand::Execute() {
    std::cout << "Listing files from archive " << archive_name_ << ": ";
}
