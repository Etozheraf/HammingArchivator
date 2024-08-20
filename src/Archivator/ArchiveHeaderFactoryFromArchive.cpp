#include <iostream>
#include <cstring>
#include "ArchiveHeaderFactoryFromArchive.h"

ArchiveHeaderFactoryFromArchive::ArchiveHeaderFactoryFromArchive(
        std::ifstream& archive, Converter& converter) : archive_(archive), converter_(converter) { }

ArchiveHeader ArchiveHeaderFactoryFromArchive::Create() {

    std::optional<std::string> opt;

    Read(opt, 9);
    std::string haf;
    if (opt.has_value()) haf = std::move(opt.value());

    Read(opt, 12);
    uint32_t control_size;

    if (opt.has_value())
        std::memcpy(&control_size, opt.value().data(), sizeof(control_size));

    Read(opt, 24);
    uint64_t header_size;
    if (opt.has_value()) header_size = std::stoull(opt.value());

    std::vector<std::string> filenames;
    std::vector<uint64_t> file_sizes;
    uint64_t archive_pos = 9 + 12 + 24;
    while (archive_pos < 3 * header_size) {
        std::string filename;
        char char_0 = '1';

        while (char_0 != '\0') {
            char_0 = '\0';

            Read(opt, 3);
            archive_pos += 3;

            if (opt.has_value()) char_0 = opt->front();
            filename.push_back(char_0);
        }
        filenames.push_back(std::move(filename));

        Read(opt, 24);
        archive_pos += 24;

        uint64_t file_size;
        if (opt.has_value()) file_size = std::stoull(opt.value());
        file_sizes.push_back(file_size);
    }
    return {haf, control_size, header_size, filenames, file_sizes};
}

void ArchiveHeaderFactoryFromArchive::Read(std::optional<std::string>& opt, uint32_t size) {
    std::unique_ptr<char[]> buf = std::make_unique<char[]>(size);
    archive_.read(static_cast<char*>(buf.get()), size);
    opt = converter_.TryConvert(static_cast<char*>(buf.get()), size);
}
