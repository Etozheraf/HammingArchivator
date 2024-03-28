#include "ConcatenateCommandBuilder.h"
#include "../Commands/ConcatenateCommand.h"

ConcatenateCommandBuilder& ConcatenateCommandBuilder::SetFirstArchiveName(
        const std::string& first_archive_name) {
    first_archive_name_ = first_archive_name;
    return *this;
}

ConcatenateCommandBuilder& ConcatenateCommandBuilder::SetSecondArchiveName(
        const std::string& second_archive_name) {
    secondArchiveName_ = second_archive_name;
    return *this;
}

std::string ConcatenateCommandBuilder::ShowErrors() {
    std::string errors;
    if (archive_name_.empty()) {
        errors += "Archive name is empty\n";
    }
    if (first_archive_name_.empty()) {
        errors += "First archive name is empty\n";
    }
    if (secondArchiveName_.empty()) {
        errors += "Second archive name is empty\n";
    }
    return errors;
}

Command* ConcatenateCommandBuilder::TryBuild() {
    if (archive_name_.empty() || first_archive_name_.empty() || secondArchiveName_.empty()) {
        return nullptr;
    }
    return new ConcatenateCommand(first_archive_name_, secondArchiveName_, archive_name_);
}
