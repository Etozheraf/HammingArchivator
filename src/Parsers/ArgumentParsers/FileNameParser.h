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
    bool Parse(std::vector<std::string>& request, std::unique_ptr<CommandBuilder>& builder) override {
        auto* concreteBuilder = dynamic_cast<Builder*>(builder.get());
        if (concreteBuilder == nullptr) {
            return false;
        }
        concreteBuilder->SetFileNames(request);
        request.clear();
        return true;
    }
};