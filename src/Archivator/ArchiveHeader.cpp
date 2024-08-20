#include "ArchiveHeader.h"

ArchiveHeader::ArchiveHeader(std::string haf, uint32_t control_bits,
                             std::vector<std::string> filenames, std::vector<uint64_t> file_sizes)
        : haf_(std::move(haf)), control_bits_(control_bits),
          filenames_(std::move(filenames)), file_sizes_(std::move(file_sizes)) {
    header_size_ = 3 + 4 + 8; // for HAF, control bits, header_size

    for (auto& filename: filenames_) {
        header_size_ += filename.size() + 1 + 8; // for name, '/0', offset
    }
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
