#pragma once

#include "../Commands/Command.hpp"
#include <string>

class CommandBuilder {
public:
    CommandBuilder& SetArchiveName(const std::string& archive_name) {
        archive_name_ = archive_name;
        return *this;
    }
    virtual std::string ShowErrors() {
        if (archive_name_.empty()) {
            return "Archive name is empty\n";
        }
        return "";
    }

    virtual Command* TryBuild() {return nullptr;}

protected:
    std::string archive_name_;
};

template<class Builder>
concept DerivedFromCommandBuilder = std::is_base_of<CommandBuilder, Builder>::value;