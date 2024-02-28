#pragma once

#include "../ChainLinkParser.hpp"
#include "../../Builders/ConcatenateCommandBuilder.h"

class MergedArchivesNameParser : public ChainLinkParserBase {
public:
    bool Parse(std::vector<std::string>& request, std::unique_ptr<CommandBuilder>& builder) override;
private:
    bool isSet = false;
};