#include "ConcatenateCommand.h"
#include <string>

ConcatenateCommand::ConcatenateCommand(
            std::string first_archive_name,
            std::string second_archive_name,
            std::string result_archive_name) :
        first_archive_name_(std::move(first_archive_name)),
        second_archive_name_(std::move(second_archive_name)),
        result_archive_name_(std::move(result_archive_name)) {}

std::string ConcatenateCommand::Execute() {
    std::cout << "Concatenating " << first_archive_name_ << " and " << second_archive_name_ << " into " << result_archive_name_ << std::endl;
}