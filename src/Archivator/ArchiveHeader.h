#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <fstream>
#include <unordered_map>
#include "../Coder/Converter.h"

class ArchiveHeader {
public:
    ArchiveHeader(uint32_t control_bits,
                  const std::vector<std::string>& filenames,
                  const std::vector<uint64_t>& file_sizes);

    ArchiveHeader(std::string haf,
                  uint32_t control_bits,
                  uint64_t header_size,
                  std::vector<std::string> filenames,
                  std::vector<uint64_t> file_sizes);

    std::string GetHaf() const;

    uint32_t GetControlBits() const;

    uint64_t GetHeaderSize() const;

    bool SetFiles(std::vector<std::string>&& filenames, std::vector<uint64_t>&& file_sizes);

    const std::vector<std::string>& GetFilenames() const;

    std::vector<std::string> GetContainedFilenamesFrom(const std::vector<std::string>& names,
                                                       std::vector<std::string>& not_contained_names) const;

    const std::vector<uint64_t>& GetFileSizes() const;

    void GetOffsets(std::unordered_map<std::string, uint64_t>& file_begins,
                    std::unordered_map<std::string, uint64_t>& file_ends) const;

    void Print(std::ofstream& archive, Converter& converter);

private:
    void CountHeaderSize();

    std::string haf_;
    uint32_t control_bits_;
    uint64_t header_size_;
    std::vector<std::string> filenames_;
    std::vector<uint64_t> file_sizes_;
};