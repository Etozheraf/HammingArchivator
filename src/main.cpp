#include <iostream>
#include <string>
#include <cstring>
#include "lib/archiver.h"

/*
header:
HAF 3
control bits 7
offset1 11
filename1 22 17
offset2 26 21
filename2 32
offset3 36
...
 */

int main(int argc, char *argv[]) {

    if (argc <= 2) {
        std::cerr << "Not enough arguments\n";
        return 1;
    }

    std::vector<std::string> files;
    std::string archive_name;
    std::string extract_file_name;
    std::string delete_file_name;
    std::string append_file_name;
    std::string merge_1archive_name;
    std::string merge_2archive_name;
    bool flag_of_create = false;
    bool flag_of_extract = false;
    bool flag_of_list = false;
    unsigned control_bits = 4;

    std::string s;
    for (int i = 1; i < argc; i++) {
        s = argv[i];
        if (s == "--create" || s == "-c") {
            flag_of_create = true;
        } else if (!strncmp(argv[i], "--file=", 7)) {
            while (*argv[i] != '=') argv[i]++;
            argv[i]++;
            archive_name = argv[i];
        } else if (s == "-f") {
            archive_name = argv[++i];
        } else if (s == "--list" || s == "-l") {
            flag_of_list = true;
        } else if (s == "--extract" || s == "-x") {
            flag_of_extract = true;
        } else if (s == "--append" || s == "-a") {
            append_file_name = argv[++i];
        } else if (s == "--delete" || s == "-d") {
            delete_file_name = argv[++i];
        } else if (s == "--concatenate" || s == "-A") {
            merge_1archive_name = argv[++i];
            merge_2archive_name = argv[++i];
        } else if (s == "--control_bits" || s == "-b") {
            control_bits = strtoul(argv[++i], nullptr, 10);
        } else if (flag_of_create) {
            files.push_back(s);
        } else if (flag_of_extract) {
            extract_file_name = argv[i];
        }
    }

    archive_name += ".haf";

    if (flag_of_create) {
        Create(archive_name, files, control_bits);
    } else if (flag_of_list) {
        List(archive_name);
    } else if (flag_of_extract) {
        if (extract_file_name.empty())
            Extract(archive_name);
        else
            Extract(archive_name, extract_file_name);
    } else if (!delete_file_name.empty()) {
        Delete(archive_name, delete_file_name);
    } else if (!append_file_name.empty()) {
        Append(archive_name, append_file_name);
    } else if (!merge_1archive_name.empty()) {
        Merge(archive_name, merge_1archive_name, merge_2archive_name);
    }
    return 0;
}
