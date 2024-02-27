#include "MergedArchivesNameParser.h"

bool MergedArchivesNameParser::Parse(std::vector<std::string>& request, CommandBuilder*& builder) {
    auto* concatenateBuilder = dynamic_cast<ConcatenateCommandBuilder*>(builder);

    if (concatenateBuilder == nullptr || request.size() < 2) {
        return false;
    }

    concatenateBuilder->SetFirstArchiveName(request[0]);
    concatenateBuilder->SetSecondArchiveName(request[1]);

    request.erase(request.begin(), request.begin() + 2);
    return true;
}