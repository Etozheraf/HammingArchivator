#include "ArchiveHeader.h"

class ArchiveHeaderFactory {
public:
    virtual std::optional<ArchiveHeader> TryCreate() = 0;
};
