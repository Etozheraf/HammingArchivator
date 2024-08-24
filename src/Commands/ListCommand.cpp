#include "ListCommand.hpp"
#include "../Coder/Converter.h"
#include "../Coder/HammingCoder.h"
#include "../Coder/ThreeBitsDecoder.h"
#include "../Archivator/ArchiveHeaderFactoryFromArchive.h"
#include <string>
#include <cstdint>
#include <fstream>
#include <deque>
#include <memory>
#include <optional>

ListCommand::ListCommand(std::string archive_name) : archive_name_(std::move(archive_name)) {}

std::string ListCommand::Execute() {
    std::cout << "Listing files from archive " << archive_name_ << "\n";

    std::ifstream archive(archive_name_, std::ios::binary);
    if (!archive.is_open()) {
        std::cerr << archive_name_ << " doesn't exist\n";
        exit(1);
    }

    Converter converter(std::make_unique<ThreeBitsDecoder>());
    ArchiveHeaderFactoryFromArchive archive_header_factory(archive, converter);
    std::optional<ArchiveHeader> archive_header_opt = archive_header_factory.TryCreate();
    if (!archive_header_opt.has_value()) {
        return "Wrong file\n";
    }
    ArchiveHeader archive_header = archive_header_opt.value();
    if (archive_header.GetHaf() != "HAF") {
        return archive_name_ + " isn't archive";
    }

    uint32_t block_size = (1 << archive_header.GetControlBits()) - 1;
    std::cout << "Block size: " << block_size << "\n";
    std::cout << "Control size: " << archive_header.GetControlBits() << "\n";
    std::cout << "Header size: " << archive_header.GetHeaderSize() << "\n";

    for (uint32_t i = 0; i < archive_header.GetFilenames().size(); ++i) {
        std::cout << "File name: " << archive_header.GetFilenames()[i] << "\n";
        std::cout << "File size: " << archive_header.GetFileSizes()[i] << "\n";
    }

    return "";
}
