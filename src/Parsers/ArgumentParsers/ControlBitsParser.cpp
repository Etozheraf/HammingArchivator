#include "ControlBitsParser.h"
#include "algorithm"

bool ControlBitsParser::Parse(std::vector<std::string>& request, std::unique_ptr<CommandBuilder>& builder) {
    auto* concrete_builder = dynamic_cast<CreateCommandBuilder*>(builder.get());
    if (concrete_builder == nullptr || request.empty()) {
        return false;
    }

    bool res = std::all_of(request[0].begin(), request[0].end(), [](char c) {
        return ('0' <= c and c <= '9');
    });

    if (!res) {
        concrete_builder->SetControlBits(0);
        request.erase(request.begin(), request.begin() + 1);
        return next_parser_->Parse(request, builder);
    };

    concrete_builder->SetControlBits(std::stoi(request[0]));
    request.erase(request.begin(), request.begin() + 1);
    return next_parser_->Parse(request, builder);
}
