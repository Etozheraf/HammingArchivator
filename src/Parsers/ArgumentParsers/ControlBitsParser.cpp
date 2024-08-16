#include "ControlBitsParser.h"

bool ControlBitsParser::Parse(std::vector<std::string>& request, std::unique_ptr<CommandBuilder>& builder) {
    auto* concrete_builder = dynamic_cast<CreateCommandBuilder*>(builder.get());
    if (concrete_builder == nullptr || request.empty()) {
        return false;
    }

    concrete_builder->SetControlBits(std::stoi(request[0]));
    request.erase(request.begin(), request.begin() + 1);
    return next_parser_->Parse(request, builder);
}
