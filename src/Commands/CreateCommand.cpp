#include "CreateCommand.h"
#include "Queue.h"
#include "../Archivator/Converter.h"
#include "../Archivator/HammingCoder.h"
#include "../Archivator/ThreeBitsCoder.h"
#include <memory>

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

    uint64_t offset = 3 + 4 + 4; // for HAF, control bits, file size

    for (auto& filename: file_names_) {
        offset += filename.size() + 1 + 4; // for name, '/0', offset
    }

    std::ofstream archive(archive_name_, std::ofstream::binary);
    Converter converter(std::make_unique<ThreeBitsCoder>());

    PrintInArchive(archive, converter.TryConvert('H'));
    PrintInArchive(archive, converter.TryConvert('A'));
    PrintInArchive(archive, converter.TryConvert('F'));
    
    PrintInArchive(archive, converter.TryConvert(control_bits_));

    std::vector<uint32_t> file_sizes;
    for (auto& filename: file_names_) {
        std::ifstream file(filename, std::ofstream::binary);

        if (!file.is_open()) {
            std::cerr << filename << " doesn't exist\n";
            continue;
        }
        
        PrintInArchive(archive, converter.TryConvert(offset));

        for (char k: filename) {
            PrintInArchive(archive, converter.TryConvert(k));
        }
        PrintInArchive(archive, converter.TryConvert('\0'));

        file.seekg(0, std::ios::end);
        file_sizes.push_back(file.tellg());
        offset += file.tellg();
        file.close();
    }

    PrintInArchive(archive, converter.TryConvert(offset));
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
