#include "ExtractCommand.h"
#include "../Coder/Converter.h"
#include "../Coder/ThreeBitsDecoder.h"
#include "../Archivator/ArchiveHeaderFactoryFromArchive.h"

ExtractCommand::ExtractCommand(std::string archive_name, std::vector<std::string> file_names)  :
        archive_name_(std::move(archive_name)),
        file_names_(std::move(file_names)) {}

std::string ExtractCommand::Execute() {
    std::cout << "Extracting files from archive " << archive_name_ << ": ";
    for (const auto& fileName : file_names_) {
        std::cout << fileName << ", ";
    }

    std::ifstream archive(archive_name_, std::ios::binary);
    if (!archive.is_open()) {
        std::cerr << archive_name_ << " doesn't exist\n";
        exit(1);
    }

    Converter converter(std::make_unique<ThreeBitsDecoder>());
    ArchiveHeaderFactoryFromArchive archive_header_factory(archive, converter);
    ArchiveHeader archive_header = archive_header_factory.Create();

    return "";
}
