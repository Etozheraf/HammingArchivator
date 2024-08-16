#pragma once

#include "Command.hpp"
#include <vector>
#include <string>
#include <cstdint>
#include <optional>
#include <fstream>

class CreateCommand : public Command {
public:
    explicit CreateCommand(std::string archive_name, std::vector<std::string> file_names);

    explicit CreateCommand(std::string archive_name, int control_bits, std::vector<std::string> file_names);

    std::string Execute() override;

private:

    void PrintInArchive(std::ofstream& archive, const std::optional<std::string>& s);

    const std::string archive_name_;
    const uint32_t control_bits_;
    const std::vector<std::string> file_names_;
};