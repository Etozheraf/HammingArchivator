#include "ConcatenateCommandBuilder.h"
#include "../Commands/ConcatenateCommand.h"

ConcatenateCommandBuilder& ConcatenateCommandBuilder::SetFirstArchiveName(
        const std::string& firstArchiveName) {
    firstArchiveName_ = firstArchiveName;
    return *this;
}

ConcatenateCommandBuilder& ConcatenateCommandBuilder::SetSecondArchiveName(
        const std::string& secondArchiveName) {
    secondArchiveName_ = secondArchiveName;
    return *this;
}

std::string ConcatenateCommandBuilder::ShowErrors() {
    std::string errors;
    if (archiveName_.empty()) {
        errors += "Archive name is empty\n";
    }
    if (firstArchiveName_.empty()) {
        errors += "First archive name is empty\n";
    }
    if (secondArchiveName_.empty()) {
        errors += "Second archive name is empty\n";
    }
    return errors;
}

Command* ConcatenateCommandBuilder::TryBuild() {
    if (archiveName_.empty() || firstArchiveName_.empty() || secondArchiveName_.empty()) {
        return nullptr;
    }
    return new ConcatenateCommand(firstArchiveName_, secondArchiveName_, archiveName_);
}
