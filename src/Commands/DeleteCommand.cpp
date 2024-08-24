#include <algorithm>
#include "DeleteCommand.h"
#include "../Coder/ThreeBitsDecoder.h"
#include "../Archivator/ArchiveHeaderFactoryFromArchive.h"
#include "../Coder/ThreeBitsCoder.h"

DeleteCommand::DeleteCommand(std::string archive_name, std::vector<std::string> file_names) :
        archive_name_(std::move(archive_name)),
        file_names_(std::move(file_names)) {}

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

    if (file_names_.empty()) {
        file_names_ = archive_header.GetFilenames();
    }

    std::unordered_map<std::string, uint64_t> file_begins;
    std::unordered_map<std::string, uint64_t> file_ends;
    CountOffsets(file_begins, file_ends, archive_header);

    ClearFilenames(archive_header);

    if (file_names_.empty()) {
        return "";
    }

    if (archive_header.GetFilenames().size() == file_names_.size()) {
        std::remove(archive_name_.data());
        return "";
    }

    std::sort(file_names_.begin(),
              file_names_.end(),
              [&file_begins](const std::string& a, const std::string& b) {
                  return file_begins[a] < file_begins[b];
              });

    archive_header.SetFiles(std::vector(file_names_), std::move(CountFileSizes(archive_header)));

    std::ofstream new_archive(archive_name_ + ".tmp", std::ios::binary);
    converter.SetCoder(std::make_unique<ThreeBitsCoder>());
    archive_header.Print(new_archive, converter);

    for (const std::string& filename : file_names_) {
        archive.seekg(file_begins[filename]);
        for (uint64_t i = file_begins[filename]; i < file_ends[filename]; ++i) {
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

void DeleteCommand::CountOffsets(std::unordered_map<std::string, uint64_t>& begins,
                                 std::unordered_map<std::string, uint64_t>& ends,
                                 const ArchiveHeader& archive_header) {
    const std::vector<std::string>& filenames = archive_header.GetFilenames();
    const std::vector<uint64_t>& file_sizes = archive_header.GetFileSizes();
    const uint64_t control_size = archive_header.GetControlBits();
    const uint64_t block_size = (1 << control_size) - 1;
    const uint64_t data_size = block_size - control_size;

    std::string prev_name = filenames[0];
    begins[prev_name] = archive_header.GetHeaderSize() * 3;

    for (int i = 0; i < filenames.size(); ++i) {
        uint64_t hamming_file_size =
                ((((file_sizes[i - 1] * 8 - 1) / data_size + 1) * block_size - 1) / 8 + 1);
        ends[filenames[i]] = begins[filenames[i]] + hamming_file_size;
        if (i > 0)
            begins[filenames[i]] = ends[prev_name];
        prev_name = filenames[i];
    }
}

void DeleteCommand::ClearFilenames(const ArchiveHeader& archive_header) {
    for (int i = static_cast<int>(file_names_.size()) - 1; i >= 0; --i) {
        auto& filename = file_names_[i];
        bool res = std::none_of(archive_header.GetFilenames().begin(),
                                archive_header.GetFilenames().end(),
                                [&filename](const std::string& s) { return s == filename; });
        if (res) {
            std::cout << file_names_[i] << " file name doesn't exist in archive\n";
            file_names_.erase(file_names_.begin() + i);
        }
    }
}

std::vector<uint64_t> DeleteCommand::CountFileSizes(const ArchiveHeader& archive_header) {
    const std::vector<std::string>& filenames = archive_header.GetFilenames();

    std::vector<uint64_t> result;
    for (const auto& file_name: file_names_) {
        auto pos = std::find(filenames.begin(), filenames.end(), file_name);
        result.push_back(archive_header.GetFileSizes()[pos - filenames.begin()]);
    }
    return std::move(result);
}
