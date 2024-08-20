#include <cstdint>
#include <vector>
#include <string>

/*
header:
HAF 3
control bits 4
offset1 8
filename1 n1
offset2 8
filename2 n2
...
file_size 8
*/

class ArchiveHeader {
public:
    ArchiveHeader(std::string haf,
                  uint32_t control_bits,
                  std::vector<std::string> filenames,
                  std::vector<uint64_t> file_sizes);

    ArchiveHeader(std::string haf,
                  uint32_t control_bits,
                  uint64_t header_size,
                  std::vector<std::string> filenames,
                  std::vector<uint64_t> file_sizes);

    uint32_t GetControlBits() const;

    uint64_t GetHeaderSize() const;

    const std::vector<std::string>& GetFilenames() const;

    const std::vector<uint64_t>& GetFileSizes() const;

private:
    std::string haf_;
    uint32_t control_bits_;
    uint64_t header_size_;
    std::vector<std::string> filenames_;
    std::vector<uint64_t> file_sizes_;
};