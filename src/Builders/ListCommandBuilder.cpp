#include "ListCommandBuilder.h"
#include "../Commands/ListCommand.hpp"

std::string ListCommandBuilder::ShowErrors() {
    if (archive_name_.empty()) {
        return "Archive name is not set";
    }
    return "";
}

Command* ListCommandBuilder::TryBuild() {
    if (archive_name_.empty()) {
        return nullptr;
    }
    return new ListCommand(archive_name_);
}