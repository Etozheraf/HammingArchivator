#include "CreateCommand.h"
#include "../Coder/Converter.h"
#include "../Coder/HammingCoder.h"
#include "../Coder/ThreeBitsCoder.h"
#include "../Archivator/ArchiveHeader.h"
#include <memory>
#include <optional>

CreateCommand::CreateCommand(std::string archive_name,
                             std::vector<std::string> file_names)
        : archive_name_(std::move(archive_name)), control_bits_(3),
          file_names_(std::move(file_names)) {}

CreateCommand::CreateCommand(std::string archive_name, int control_bits,
                             std::vector<std::string> file_names)
        : archive_name_(std::move(archive_name)), control_bits_(control_bits),
          file_names_(std::move(file_names)) {}

std::string CreateCommand::Execute() {
    std::ofstream archive(archive_name_, std::ofstream::binary);
    Converter converter(std::make_unique<ThreeBitsCoder>());

    std::vector<uint64_t> file_sizes;
    file_sizes.reserve(file_names_.size());
    for (auto& filename: file_names_) {
        std::ifstream file(filename, std::ofstream::binary);

        if (!file.is_open()) {
            std::cout << filename << " doesn't exist\n";
            continue;
        }

        file.seekg(0, std::ios::end);
        file_sizes.push_back(file.tellg());
        file.close();
    }
    ArchiveHeader archive_head(control_bits_, file_names_, file_sizes);
    archive_head.Print(archive, converter);

    converter.SetCoder(std::make_unique<HammingCoder>(control_bits_));

    for (uint32_t i = 0; i < file_sizes.size(); ++i) {
        std::ifstream file(file_names_[i], std::ofstream::binary);

        for (int j = 0; j < file_sizes[i]; ++j) {
            uint8_t buf = 0;
            file.read((char*) &buf, sizeof(buf));
            auto coded_buf = converter.TryConvert(buf);
            if (!coded_buf.has_value()) continue;
            archive.write(coded_buf.value().c_str(), coded_buf.value().size());
        }

        archive << converter.GetRemainder();
    }
    return "";
}