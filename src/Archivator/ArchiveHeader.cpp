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

void ArchiveHeader::GetOffsets(std::unordered_map<std::string, uint64_t>& file_begins,
                               std::unordered_map<std::string, uint64_t>& file_ends) const {

    const int block_size = (1 << control_bits_) - 1;
    const int data_size = block_size - control_bits_;

    file_begins[filenames_[0]] = header_size_ * 3;

    for (int i = 0; i < filenames_.size() - 1; ++i) {
        uint64_t hamming_file_size =
                ((((file_sizes_[i] * 8 - 1) / data_size + 1) * block_size - 1) / 8 + 1);

        file_ends[filenames_[i]] = file_begins[filenames_[i]] + hamming_file_size;
        file_begins[filenames_[i + 1]] = file_ends[filenames_[i]];
    }
    uint64_t hamming_file_size =
            ((((file_sizes_.back() * 8 - 1) / data_size + 1) * block_size - 1) / 8 + 1);
    file_ends[filenames_.back()] = file_begins[filenames_.back()] + hamming_file_size;
}

std::vector<std::string> ArchiveHeader::GetContainedFilenamesFrom(const std::vector<std::string>& names,
                                                                  std::vector<std::string>& not_contained_names) const {
    std::unordered_map<std::string, bool> is_have;
    for (const auto& filename: names) {
        is_have[filename] = false;
    }
    for (const auto& filename: filenames_) {
        is_have[filename] = true;
    }

    std::vector<std::string> answer;
    answer.reserve(names.size());
    for (auto& name: names) {
        if (is_have[name])
            answer.push_back(name);
        else
            not_contained_names.push_back(name);
    }
    return std::move(answer);
}
