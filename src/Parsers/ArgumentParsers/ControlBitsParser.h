#pragma once
#include "../ChainLinkParser.hpp"
#include "../../Builders/CreateCommandBuilder.h"


class ControlBitsParser : public ChainLinkParserBase {
    bool Parse(std::vector<std::string>& request, std::unique_ptr<CommandBuilder>& builder) override;
};
