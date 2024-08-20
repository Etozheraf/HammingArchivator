#include "ArchiveHeaderFactory.h"
#include "../Coder/Converter.h"
#include<fstream>

class ArchiveHeaderFactoryFromArchive : public ArchiveHeaderFactory {
public:
    ArchiveHeaderFactoryFromArchive(std::ifstream& archive, Converter& converter);

    ArchiveHeader Create() override;

private:

    void Read(std::optional<std::string>& opt, uint32_t size);

    std::ifstream& archive_;
    Converter& converter_;
};