#pragma once

#include "CommandBuilder.hpp"
#include <vector>

template<DerivedFromCommand DerivedCommand>
class CommandWithFileNameBuilder : public CommandBuilder {
public:
    explicit CommandWithFileNameBuilder(const CommandBuilder& c) : CommandBuilder(c) {};

    CommandWithFileNameBuilder& SetFileNames(const std::vector<std::string>& file_names);

    std::string ShowErrors() override;

    DerivedCommand* TryBuild() override;

private:
    std::vector<std::string> file_names_;
};


template<DerivedFromCommand DerivedCommand>
CommandWithFileNameBuilder<DerivedCommand>&
CommandWithFileNameBuilder<DerivedCommand>::SetFileNames(const std::vector<std::string>& file_names) {
    file_names_ = file_names;
    return *this;
}

template<DerivedFromCommand DerivedCommand>
std::string CommandWithFileNameBuilder<DerivedCommand>::ShowErrors() {
    std::string errors;
    if (archive_name_.empty())
        errors += "Archive name is empty\n";
    if (file_names_.empty())
        errors += "File names are empty\n";
    return errors;
}

template<DerivedFromCommand DerivedCommand>
DerivedCommand* CommandWithFileNameBuilder<DerivedCommand>::TryBuild() {
    if (archive_name_.empty() || file_names_.empty())
        return nullptr;

    return new DerivedCommand(archive_name_, file_names_);
}