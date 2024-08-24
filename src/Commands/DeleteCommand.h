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
    void CountOffsets(
            std::unordered_map<std::string, uint64_t>& begins,
            std::unordered_map<std::string, uint64_t>& ends,
            const ArchiveHeader& archive_header);

    std::vector<uint64_t> CountFileSizes(const ArchiveHeader& archive_header);

    void ClearFilenames(const ArchiveHeader& archive_header);

    std::string archive_name_;
    std::vector<std::string> file_names_;
};