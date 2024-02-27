#pragma once

#include "../ChainLinkParser.hpp"
#include "../../Builders/ConcatenateCommandBuilder.h"

class MergedArchivesNameParser : public ChainLinkParserBase {
    bool Parse(std::vector<std::string> &request, CommandBuilder *&builder) override;
};