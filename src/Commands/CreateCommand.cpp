#include "CreateCommand.h"
#include "Queue.h"
#include "../Coder/Converter.h"
#include "../Coder/HammingCoder.h"
#include "../Coder/ThreeBitsCoder.h"
#include <memory>
#include <optional>

/*
header:
HAF 3
control bits 4
offset1 8
filename1 n1
offset2 8
filename2 n2
...
file_size 8
*/

CreateCommand::CreateCommand(std::string archive_name,
                             std::vector<std::string> file_names)
        : archive_name_(std::move(archive_name)), control_bits_(3),
          file_names_(std::move(file_names)) {}

CreateCommand::CreateCommand(std::string archive_name, int control_bits,
                             std::vector<std::string> file_names)
        : archive_name_(std::move(archive_name)), control_bits_(control_bits),
          file_names_(std::move(file_names)) {}

std::string CreateCommand::Execute() {

    uint64_t header_size = 3 + 4 + 4; // for HAF, control bits, file size

    for (auto& filename: file_names_) {
        header_size += filename.size() + 1 + 4; // for name, '/0', header_size
    }

    std::ofstream archive(archive_name_, std::ofstream::binary);
    Converter converter(std::make_unique<ThreeBitsCoder>());

    PrintInArchive(archive, converter.TryConvert("HAF", 3));
    
    PrintInArchive(archive, converter.TryConvert(control_bits_));

    PrintInArchive(archive, converter.TryConvert(header_size));

    std::vector<uint64_t> file_sizes;
    file_sizes.reserve(file_names_.size());
    for (auto& filename: file_names_) {
        std::ifstream file(filename, std::ofstream::binary);

        if (!file.is_open()) {
            std::cerr << filename << " doesn't exist\n";
            continue;
        }

        for (char k: filename) {
            PrintInArchive(archive, converter.TryConvert(k));
        }
        PrintInArchive(archive, converter.TryConvert('\0'));

        file.seekg(0, std::ios::end);
        file_sizes.push_back(file.tellg());
        PrintInArchive(archive, converter.TryConvert(file_sizes.back()));
        file.close();
    }

    converter.SetCoder(std::make_unique<HammingCoder>(control_bits_));

    for (uint32_t file_size : file_sizes) {
        std::ifstream file(file_names_.front(), std::ofstream::binary);

        for (int i = 0; i < file_size; ++i) {
            uint8_t buf = 0;
            file.read((char*) &buf, sizeof(buf));
            PrintInArchive(archive, converter.TryConvert(buf));
        }

        archive << converter.GetRemainder();
    }
    return "";
}

void CreateCommand::PrintInArchive(std::ofstream& archive, const std::optional<std::string>& s) {
    if (s.has_value()) {
        archive << s.value();
    }
}
