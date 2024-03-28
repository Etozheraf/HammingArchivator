#include "MergedArchivesNameParser.h"

bool MergedArchivesNameParser::Parse(std::vector<std::string>& request, std::unique_ptr<CommandBuilder>& builder) {
    if (is_set) return false;
    is_set = true;

    auto* concatenate_builder(dynamic_cast<ConcatenateCommandBuilder*>(builder.get()));

    if (concatenate_builder == nullptr || request.size() < 2) {
        return false;
    }

    concatenate_builder->SetFirstArchiveName(request[0]);
    concatenate_builder->SetSecondArchiveName(request[1]);

    request.erase(request.begin(), request.begin() + 2);
    return true;
}