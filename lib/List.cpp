#include <iostream>
#include <string>
#include <fstream>
#include "archiver.h"


void List(const std::string &archive_name) {
    std::ifstream archive(archive_name, std::ios::binary);
    if (!archive.is_open()) {
        std::cerr << archive_name << " doesn't exist\n";
        exit(1);
    }

    Queue total(97);

    uint8_t buf = 0;
    ReadNBytes(archive, total, 9);

    uint8_t ch[3];
    for (unsigned char &c: ch) {
        c = 0;
        for (int j = 0; j < 8; j++) {
            Correction(total, 3);
            c <<= 1;
            c += total.dequeue();
            total.dequeue();
            total.dequeue();
        }
    }

    if (ch[0] != 'H' || ch[1] != 'A' || ch[2] != 'F') {
        std::cerr << archive_name << " isn't an archive";
        exit(1);
    }

    ReadNBytes(archive, total, 12);

    unsigned size_of_control = 0;
    UnsignedFromQueue(total, size_of_control);

    ReadNBytes(archive, total, 12);

    unsigned size_of_header = 0;
    UnsignedFromQueue(total, size_of_header);

    std::cout << "Files in " << archive_name << '\n';

    uint8_t char_for_null;
    std::string s;
    unsigned position_in_archive = archive.tellg();
    while (position_in_archive < 3 * size_of_header) {
        s = "";
        char_for_null = 1;
        while (char_for_null != '\0') {

            position_in_archive += 3;
            ReadNBytes(archive, total, 3);

            CharFromQueue(total, char_for_null);

            s += char(char_for_null);
        }
        std::cout << s << "\n";

        position_in_archive += 12;
        ReadNBytes(archive, total, 12);

        unsigned offset = 0;
        UnsignedFromQueue(total, offset);
    }
}
