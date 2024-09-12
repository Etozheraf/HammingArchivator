#include <algorithm>
#include "ExtractCommand.h"
#include "../Coder/Converter.h"
#include "../Coder/ThreeBitsDecoder.h"
#include "../Archivator/ArchiveHeaderFactoryFromArchive.h"
#include "../Coder/HammingDecoder.h"

ExtractCommand::ExtractCommand(std::string archive_name, std::vector<std::string> file_names) :
        archive_name_(std::move(archive_name)),
        extracted_filenames_(std::move(file_names)) {}

std::string ExtractCommand::Execute() {
    std::cout << "Extracting files from archive " << archive_name_ << ": ";
    for (const auto& fileName: extracted_filenames_) {
        std::cout << fileName << ", ";
    }
    std::cout << "\n";

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

    std::unordered_map<std::string, uint64_t> file_begins;
    std::unordered_map<std::string, uint64_t> file_ends;
    archive_header.GetOffsets(file_begins, file_ends);

    std::vector<std::string> not_contained_filenames;
    extracted_filenames_ = archive_header.GetContainedFilenamesFrom(extracted_filenames_, not_contained_filenames);
    for (const auto& not_contained_filename: not_contained_filenames) {
        std::cout << not_contained_filename << " file isn't in archive";
    }

    if (extracted_filenames_.empty()) {
        return "";
    }

    converter.SetCoder(std::make_unique<HammingDecoder>(archive_header.GetControlBits()));
    for (auto& extracted_filename: extracted_filenames_) {
        archive.seekg(file_begins[extracted_filename]);
        std::ofstream file(extracted_filename, std::ios::binary);
        for (uint64_t i = file_begins[extracted_filename]; i < file_ends[extracted_filename]; ++i) {
            char buf;
            archive >> buf;
            auto decoded_buf = converter.TryConvert(static_cast<uint8_t>(buf));
            if (!decoded_buf.has_value()) {
                continue;
            }
            file << decoded_buf.value();
        }
        converter.GetRemainder();
    }

    return "";
}
