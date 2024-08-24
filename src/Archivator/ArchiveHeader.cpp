#include "ArchiveHeader.h"

ArchiveHeader::ArchiveHeader(uint32_t control_bits,
                             const std::vector<std::string>& filenames,
                             const std::vector<uint64_t>& file_sizes)
        : haf_("HAF"), control_bits_(control_bits),
          filenames_(filenames), file_sizes_(file_sizes) {
    CountHeaderSize();
}

ArchiveHeader::ArchiveHeader(std::string haf, uint32_t control_bits, uint64_t header_size,
                             std::vector<std::string> filenames, std::vector<uint64_t> file_sizes)
        : haf_(std::move(haf)), control_bits_(control_bits), header_size_(header_size),
          filenames_(std::move(filenames)), file_sizes_(std::move(file_sizes)) {}

uint32_t ArchiveHeader::GetControlBits() const {
    return control_bits_;
}

uint64_t ArchiveHeader::GetHeaderSize() const {
    return header_size_;
}

const std::vector<std::string>& ArchiveHeader::GetFilenames() const {
    return filenames_;
}

const std::vector<uint64_t>& ArchiveHeader::GetFileSizes() const {
    return file_sizes_;
}

bool ArchiveHeader::SetFiles(std::vector<std::string>&& filenames, std::vector<uint64_t>&& file_sizes) {
    if (filenames.size() != file_sizes.size()) return false;
    filenames_ = filenames;
    file_sizes_ = file_sizes;
    CountHeaderSize();
    return true;
}

void ArchiveHeader::CountHeaderSize() {
    header_size_ = 3 + 4 + 8; // for HAF, control bits, header_size

    for (auto& filename: filenames_) {
        header_size_ += filename.size() + 1 + 8; // for name, '/0', offset
    }
}

void ArchiveHeader::Print(std::ofstream& archive, Converter& converter) {
    archive << converter.TryConvert("HAF", 3).value();

    archive << converter.TryConvert(control_bits_).value();

    archive << converter.TryConvert(header_size_).value();

    for (int i = 0; i < filenames_.size(); ++i) {
        archive << converter.TryConvert(filenames_[i].c_str(), filenames_[i].size()).value();
        archive << converter.TryConvert(static_cast<uint8_t>('\0')).value();
        archive << converter.TryConvert(file_sizes_[i]).value();
    }
}

std::string ArchiveHeader::GetHaf() const {
    return haf_;
}
