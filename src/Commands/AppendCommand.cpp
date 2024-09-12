#include <fstream>
#include "AppendCommand.h"
#include "../Coder/Converter.h"
#include "../Coder/ThreeBitsDecoder.h"
#include "../Archivator/ArchiveHeaderFactoryFromArchive.h"
#include "../Coder/ThreeBitsCoder.h"
#include "../Coder/HammingCoder.h"

AppendCommand::AppendCommand(std::string archive_name, std::vector<std::string> file_names) :
        archive_name_(std::move(archive_name)),
        appended_filenames_(std::move(file_names)) {}

std::string AppendCommand::Execute() {
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

    if (appended_filenames_.empty()) {
        return "";
    }

    std::vector<std::string> not_contained_filenames;
    std::vector<std::string> contained_filenames = archive_header.GetContainedFilenamesFrom(
            appended_filenames_,
            not_contained_filenames);
    appended_filenames_ = std::move(not_contained_filenames);

    for (const auto& contained_filename: contained_filenames) {
        std::cout << "File " + contained_filename + " is already in archive\n";
    }

    if (appended_filenames_.empty()) {
        return "";
    }

    std::vector<uint64_t> appended_file_sizes;
    appended_file_sizes.reserve(appended_filenames_.size());
    for (int i = 0; i < appended_filenames_.size(); ++i) {
        std::ifstream file(appended_filenames_[i], std::ofstream::binary);

        if (!file.is_open()) {
            std::cout << appended_filenames_[i] << " doesn't exist\n";
            appended_filenames_.erase(appended_filenames_.begin() + i);
            --i;
            continue;
        }

        file.seekg(0, std::ios::end);
        appended_file_sizes.push_back(file.tellg());
        file.close();
    }

    std::vector<std::string> new_filenames = archive_header.GetFilenames();
    std::vector<uint64_t> new_file_sizes = archive_header.GetFileSizes();
    new_filenames.insert(new_filenames.end(),
                         appended_filenames_.begin(),
                         appended_filenames_.end());
    new_file_sizes.insert(new_file_sizes.end(),
                          appended_file_sizes.begin(),
                          appended_file_sizes.end());

    archive_header.SetFiles(std::move(new_filenames), std::move(new_file_sizes));

    std::ofstream new_archive(archive_name_ + ".tmp", std::ios::binary);
    converter.SetCoder(std::make_unique<ThreeBitsCoder>());
    archive_header.Print(new_archive, converter);


    while (!archive.eof()) {
        char buf;
        archive >> buf;
        if (archive.eof()) break;
        new_archive << buf;
    }

    converter.SetCoder(std::make_unique<HammingCoder>(archive_header.GetControlBits()));

    for (uint32_t i = 0; i < appended_filenames_.size(); ++i) {
        std::ifstream file(appended_filenames_[i], std::ofstream::binary);

        for (int j = 0; j < appended_file_sizes[i]; ++j) {
            uint8_t buf = 0;
            file.read((char*) &buf, sizeof(buf));
            auto coded_buf = converter.TryConvert(buf);
            if (!coded_buf.has_value()) continue;
            new_archive.write(coded_buf.value().c_str(), coded_buf.value().size());
        }

        new_archive << converter.GetRemainder();
    }

    archive.close();
    new_archive.close();

    std::remove(archive_name_.data());
    std::rename((archive_name_ + ".tmp").data(), archive_name_.data());

    return "";
}
