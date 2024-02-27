#pragma once

#include "../Commands/Command.hpp"
#include <string>

class CommandBuilder {
public:
    CommandBuilder& SetArchiveName(const std::string& archiveName) {
        archiveName_ = archiveName;
        return *this;
    }
    virtual std::string ShowErrors() {return "";}

    virtual Command* TryBuild() {return nullptr;}

protected:
    std::string archiveName_;
};

template<class Builder>
concept DerivedFromCommandBuilder = std::is_base_of<CommandBuilder, Builder>::value;