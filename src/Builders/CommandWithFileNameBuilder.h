#pragma once

#include "CommandBuilder.hpp"
#include <vector>

template<DerivedFromCommand DerivedCommand>
class CommandWithFileNameBuilder : public CommandBuilder {
public:
    explicit CommandWithFileNameBuilder(const CommandBuilder& c) : CommandBuilder(c) {};

    CommandWithFileNameBuilder& SetFileNames(const std::vector<std::string>& fileNames);

    std::string ShowErrors() override;

    DerivedCommand* TryBuild() override;

private:
    std::vector<std::string> fileNames_;
};


template<DerivedFromCommand DerivedCommand>
CommandWithFileNameBuilder<DerivedCommand>&
CommandWithFileNameBuilder<DerivedCommand>::SetFileNames(const std::vector<std::string>& fileNames) {
    fileNames_ = fileNames;
    return *this;
}

template<DerivedFromCommand DerivedCommand>
std::string CommandWithFileNameBuilder<DerivedCommand>::ShowErrors() {
    std::string errors;
    if (archiveName_.empty())
        errors += "Archive name is empty\n";
    if (fileNames_.empty())
        errors += "File names are empty\n";
    return errors;
}

template<DerivedFromCommand DerivedCommand>
DerivedCommand* CommandWithFileNameBuilder<DerivedCommand>::TryBuild() {
    if (archiveName_.empty() || fileNames_.empty())
        return nullptr;

    return new DerivedCommand(archiveName_, fileNames_);
}