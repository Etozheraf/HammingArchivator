#include "ArchiveHeaderFactory.h"
#include "../Coder/Converter.h"
#include<fstream>

class ArchiveHeaderFactoryFromArchive : public ArchiveHeaderFactory {
public:
    ArchiveHeaderFactoryFromArchive(std::ifstream& archive, Converter& converter);

    std::optional<ArchiveHeader> TryCreate() override;

private:
    std::optional<std::string> Read(uint32_t size);

    uint64_t GetUintFromString(const std::string& str);

    std::ifstream& archive_;
    Converter& converter_;
};