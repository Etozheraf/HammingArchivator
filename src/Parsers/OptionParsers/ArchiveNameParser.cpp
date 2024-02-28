#include "ArchiveNameParser.h"

bool ArchiveNameParser::Parse(std::vector<std::string>& request, std::unique_ptr<CommandBuilder>& builder) {
    if (request[0].starts_with("--file=")) {
        builder->SetArchiveName(request[0].substr(7));
        request.erase(request.begin());
        return true;
    }

    if (request.size() >= 2 && request[0].starts_with('-') && request[0].ends_with('f')) {
        builder->SetArchiveName(request[1]);
        request.erase(request.begin() + 1);

        request[0].pop_back();
        if (request[0] == "-") request.erase(request.begin());

        return true;
    }

    if (next_parser_ == nullptr)
        return false;

    return next_parser_->Parse(request, builder);
}
