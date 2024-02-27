#include "ListCommandBuilder.h"
#include "../Commands/ListCommand.hpp"

std::string ListCommandBuilder::ShowErrors() {
    if (archiveName_.empty()) {
        return "Archive name is not set";
    }
    return "";
}

Command* ListCommandBuilder::TryBuild() {
    if (archiveName_.empty()) {
        return nullptr;
    }
    return new ListCommand(archiveName_);
}