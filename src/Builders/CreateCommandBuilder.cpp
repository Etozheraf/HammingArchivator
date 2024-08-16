#include "CreateCommandBuilder.h"

CreateCommandBuilder& CreateCommandBuilder::SetControlBits(int control_bits) {
    control_bits_ = control_bits;
    return *this;
}

std::string CreateCommandBuilder::ShowErrors() {
    std::string errors;
    if (archive_name_.empty())
        errors += "Archive name is empty\n";
    if (file_names_.empty())
        errors += "File names are empty\n";
    if (control_bits_ == 0)
        errors += "Control bits don't initialized\n";
    return errors;
}

CreateCommand* CreateCommandBuilder::TryBuild() {
    if (archive_name_.empty() || file_names_.empty() || control_bits_ == 0)
        return nullptr;

    return new CreateCommand(archive_name_, control_bits_, file_names_);
}