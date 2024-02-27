#pragma once

#include "../ChainLinkParser.hpp"

template<class Builder>
concept BuilderWithFileName = DerivedFromCommandBuilder<Builder>
                              && requires(Builder builder, std::vector<std::string> fileNames)
{
    builder.SetFileNames(fileNames);
};

template<BuilderWithFileName Builder>
class FileNameParser : public ChainLinkParserBase {
public:
    bool Parse(std::vector<std::string>& request, CommandBuilder*& builder) override;
};

template<BuilderWithFileName Builder>
bool FileNameParser<Builder>::Parse(std::vector<std::string>& request, CommandBuilder*& builder) {
    auto* concreteBuilder = dynamic_cast<Builder*>(builder);
    if (concreteBuilder == nullptr) {
        return false;
    }
    concreteBuilder->SetFileNames(request);
    request.clear();
    return true;
}