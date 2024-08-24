#include <iostream>
#include <cstring>
#include "ArchiveHeaderFactoryFromArchive.h"

ArchiveHeaderFactoryFromArchive::ArchiveHeaderFactoryFromArchive(
        std::ifstream& archive, Converter& converter) : archive_(archive), converter_(converter) { }

std::optional<ArchiveHeader> ArchiveHeaderFactoryFromArchive::TryCreate() {
    std::optional<std::string> opt;

    opt = std::move(Read(9));
    if (!opt.has_value()) return {};
    std::string haf = std::move(opt.value());

    opt = std::move(Read(12));
    if (!opt.has_value()) return {};
    uint32_t control_size = GetUintFromString(opt.value());

    opt = std::move(Read(24));
    if (!opt.has_value()) return {};
    uint64_t header_size = GetUintFromString(opt.value());

    std::vector<std::string> filenames;
    std::vector<uint64_t> file_sizes;
    uint64_t archive_pos = 9 + 12 + 24;
    while (archive_pos < 3 * header_size) {
        std::string filename;
        char char_0 = '1';

        while (char_0 != '\0') {
            char_0 = '\0';

            opt = std::move(Read(3));
            if (!opt.has_value()) return {};
            archive_pos += 3;

            char_0 = opt->front();
            filename.push_back(char_0);
        }
        filenames.push_back(std::move(filename));

        opt = std::move(Read(24));
        if (!opt.has_value()) return {};
        archive_pos += 24;

        uint64_t file_size = GetUintFromString(opt.value());
        file_sizes.push_back(file_size);
    }
    return ArchiveHeader(haf, control_size, header_size, std::move(filenames), std::move(file_sizes));
}

std::optional<std::string> ArchiveHeaderFactoryFromArchive::Read(uint32_t size) {
    std::unique_ptr<char[]> buf = std::make_unique<char[]>(size);
    archive_.read(static_cast<char*>(buf.get()), size);
    return std::move(converter_.TryConvert(static_cast<char*>(buf.get()), size));
}

uint64_t ArchiveHeaderFactoryFromArchive::GetUintFromString(const std::string& str) {
    uint64_t res = 0;
    for (char c : str) {
        res *= 256;
        res += static_cast<uint8_t>(c);
    }
    return res;
}
