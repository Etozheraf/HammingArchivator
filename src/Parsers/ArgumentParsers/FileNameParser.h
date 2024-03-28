#pragma once

#include "../ChainLinkParser.hpp"

template<class Builder>
concept BuilderWithFileName = DerivedFromCommandBuilder<Builder>
                              && requires(Builder builder, std::vector<std::string> file_names)
{
    builder.SetFileNames(file_names);
};

template<BuilderWithFileName Builder>
class FileNameParser : public ChainLinkParserBase {
public:
    bool Parse(std::vector<std::string>& request, std::unique_ptr<CommandBuilder>& builder) override {
        auto* concrete_builder = dynamic_cast<Builder*>(builder.get());
        if (concrete_builder == nullptr) {
            return false;
        }
        concrete_builder->SetFileNames(request);
        request.clear();
        return true;
    }
};