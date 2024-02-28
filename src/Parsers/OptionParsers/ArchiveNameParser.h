#pragma once

#include "../ChainLinkParser.hpp"

class ArchiveNameParser : public ChainLinkParserBase {
public:
    bool Parse(std::vector<std::string>& request, std::unique_ptr<CommandBuilder>& builder) override;
};
