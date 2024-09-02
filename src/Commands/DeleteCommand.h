#pragma once

#include "Command.hpp"
#include "../Archivator/ArchiveHeader.h"
#include <vector>
#include <string>
#include <cstdint>
#include <unordered_map>

class DeleteCommand : public Command {
public:
    explicit DeleteCommand(std::string archive_name, std::vector<std::string> file_names);

    std::string Execute() override;

private:

    void UpdateArchiveHeader(ArchiveHeader& archive_header);

    std::string archive_name_;
    std::vector<std::string> deleted_filenames_;
};