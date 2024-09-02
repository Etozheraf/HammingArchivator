#include <algorithm>
#include "DeleteCommand.h"
#include "../Coder/ThreeBitsDecoder.h"
#include "../Archivator/ArchiveHeaderFactoryFromArchive.h"
#include "../Coder/ThreeBitsCoder.h"

DeleteCommand::DeleteCommand(std::string archive_name, std::vector<std::string> file_names) :
        archive_name_(std::move(archive_name)),
        deleted_filenames_(std::move(file_names)) {}

std::string DeleteCommand::Execute() {
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

    if (deleted_filenames_.empty()) {
        deleted_filenames_ = archive_header.GetFilenames();
    }

    std::unordered_map<std::string, uint64_t> file_begins;
    std::unordered_map<std::string, uint64_t> file_ends;
    archive_header.GetOffsets(file_begins, file_ends);

    deleted_filenames_ = archive_header.GetContainedFilenamesFrom(deleted_filenames_);

    if (deleted_filenames_.empty()) {
        return "";
    }

    if (deleted_filenames_.size() == archive_header.GetFilenames().size()) {
        std::remove(archive_name_.data());
        return "";
    }

    UpdateArchiveHeader(archive_header);

    std::ofstream new_archive(archive_name_ + ".tmp", std::ios::binary);
    converter.SetCoder(std::make_unique<ThreeBitsCoder>());
    archive_header.Print(new_archive, converter);

    for (auto& remaining_filename: archive_header.GetFilenames()) {
        archive.seekg(file_begins[remaining_filename]);
        for (uint64_t j = file_begins[remaining_filename]; j < file_ends[remaining_filename]; ++j) {
            char buf;
            archive >> buf;
            new_archive << buf;
        }
    }

    archive.close();
    new_archive.close();

    std::remove(archive_name_.data());
    std::rename((archive_name_ + ".tmp").data(), archive_name_.data());
    return "";
}

void DeleteCommand::UpdateArchiveHeader(ArchiveHeader& archive_header) {
    const std::vector<std::string>& filenames = archive_header.GetFilenames();

    std::vector<std::string> remaining_filenames;
    std::vector<uint64_t> remaining_file_sizes;

    for (int i = 0; i < filenames.size(); ++i) {
        if (std::find(deleted_filenames_.begin(), deleted_filenames_.end(), filenames[i]) !=
            deleted_filenames_.end())
            continue;
        remaining_filenames.push_back(filenames[i]);
        remaining_file_sizes.push_back(archive_header.GetFileSizes()[i]);
    }

    archive_header.SetFiles(std::move(remaining_filenames), std::move(remaining_file_sizes));
}
