#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>
#include "archiver.h"


void Extract(const std::string &archive_name) {
    std::ifstream archive(archive_name, std::ios::binary);
    if (!archive.is_open()) {
        std::cerr << archive_name << " doesn't exist\n";
        exit(1);
    }

    Queue total(97);

    uint8_t buf = 0;
    ReadNBytes(archive, total, 9);

    uint8_t ch[3];
    for (uint8_t i = 0; i < 3; i++) {
        ch[i] = 1;
        for (int j = 0; j < 8; j++) {
            Correction(total, 3);
            ch[i] <<= 1;
            ch[i] += total.dequeue();
            total.dequeue();
            total.dequeue();
        }
    }
    if (ch[0] != 'H' || ch[1] != 'A' || ch[2] != 'F') {
        std::cerr << "this isn't an archive";
        exit(1);
    }

    ReadNBytes(archive, total, 12);

    unsigned size_of_control = 0;
    UnsignedFromQueue(total, size_of_control);

    ReadNBytes(archive, total, 12);

    unsigned size_of_header = 0;
    UnsignedFromQueue(total, size_of_header);

    uint8_t char_for_null;
    std::string s;
    std::vector<std::string> filenames;
    std::vector<unsigned> offset_of_file;
    offset_of_file.push_back(size_of_header);
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
        filenames.push_back(s);

        position_in_archive += 12;
        ReadNBytes(archive, total, 12);

        unsigned offset = 0;
        UnsignedFromQueue(total, offset);
        offset_of_file.push_back(offset);
    }

    std::vector<unsigned> size_of_file(offset_of_file.size() - 1);
    for (int i = 0; i < offset_of_file.size() - 1; i++) {
        size_of_file.at(i) = (offset_of_file.at(i + 1) - offset_of_file.at(i));
    }

    delete[] total.bit;
    unsigned size_of_total = (1 << size_of_control) - 1;


    unsigned size_of_data = size_of_total - size_of_control;
    Queue data(2 * size_of_data);

    while (!filenames.empty()) {
        std::ofstream file(filenames.front(), std::ios::binary);
        filenames.erase(filenames.begin());
        offset_of_file.erase(offset_of_file.begin());

        unsigned size_of_file_after_Hamming = ((size_of_file.front() * 8 + size_of_data - 1) / size_of_data);
        size_of_file_after_Hamming = (size_of_file_after_Hamming * size_of_total + 7) / 8;

        position_in_archive = 0;
        unsigned position_in_file = 0;

        total.bit = new bool[2 * size_of_total];
        total.length = 2 * size_of_total;
        total.head = 0;
        total.tail = 0;

        data.bit = new bool[2 * size_of_data];
        data.length = 2 * size_of_data;
        data.head = 0;
        data.tail = 0;

        while (position_in_archive < size_of_file_after_Hamming && position_in_file < size_of_file.front()) {

            while (total.size() < size_of_total && position_in_archive < size_of_file_after_Hamming) {
                archive.read((char *) &buf, sizeof(buf));
                position_in_archive++;
                for (uint8_t j = 128; j > 0; j >>= 1) {
                    total.push((buf & j) / j);
                }
            }

            while (total.size() >= size_of_total) {
                Correction(total, size_of_total);
                for (int i = 0; i < size_of_total; ++i) {
                    if (log2(i + 1) - unsigned(log2(i + 1)) != 0.0) {
                        data.push(total.dequeue());
                    } else {
                        total.dequeue();
                    }
                }
            }

            while (data.size() > 7 && position_in_file < size_of_file.front()) {
                buf = 0;
                for (int j = 0; j < 8; j++) {
                    buf = (buf << 1) + data.dequeue();
                }
                file.write((char *) &buf, sizeof(buf));
                position_in_file++;
            }
        }
        delete[] total.bit;
        delete[] data.bit;
        size_of_file.erase(size_of_file.begin());
    }
}

void Extract(const std::string &archive_name, const std::string &file_to_extract) {
    std::ifstream archive(archive_name, std::ios::binary);
    if (!archive.is_open()) {
        std::cerr << archive_name << " doesn't exist\n";
        exit(1);
    }

    Queue total(97);

    uint8_t buf = 0;
    ReadNBytes(archive, total, 9);
    uint8_t ch[3];
    for (uint8_t i = 0; i < 3; i++) {
        ch[i] = 1;
        for (int j = 0; j < 8; j++) {
            Correction(total, 3);
            ch[i] <<= 1;
            ch[i] += total.dequeue();
            total.dequeue();
            total.dequeue();
        }
    }
    if (ch[0] != 'H' || ch[1] != 'A' || ch[2] != 'F') {
        std::cerr << "this isn't an archive";
        exit(1);
    }

    ReadNBytes(archive, total, 12);

    unsigned size_of_control = 0;
    UnsignedFromQueue(total, size_of_control);

    ReadNBytes(archive, total, 12);

    unsigned size_of_header = 0;
    UnsignedFromQueue(total, size_of_header);

    uint8_t char_for_null;
    std::string s;
    bool file_exist = false;
    unsigned offset_of_file;
    unsigned offset_of_next_file = size_of_header;
    unsigned offset_of_file_after_Hamming = 3 * size_of_header;
    unsigned size_of_file;
    unsigned size_of_file_after_Hamming;
    unsigned size_of_total = (1 << size_of_control) - 1;
    unsigned size_of_data = size_of_total - size_of_control;
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
        position_in_archive += 12;
        ReadNBytes(archive, total, 12);

        unsigned offset = 0;
        UnsignedFromQueue(total, offset);

        offset_of_file = offset_of_next_file;
        offset_of_next_file = offset;
        size_of_file = offset_of_next_file - offset_of_file;
        size_of_file_after_Hamming = ((size_of_file * 8 + size_of_data - 1) / size_of_data);
        size_of_file_after_Hamming = (size_of_file_after_Hamming * size_of_total + 7) / 8;
        if (s == (file_to_extract + '\0')) {
            file_exist = true;
            break;
        }
        offset_of_file_after_Hamming += size_of_file_after_Hamming;
    }
    delete[] total.bit;

    if (!file_exist) {
        std::cerr << file_to_extract << " is not in " << archive_name;
        exit(1);
    }

    Queue data(2 * size_of_data);


    std::ofstream file(file_to_extract, std::ios::binary);

    archive.seekg(offset_of_file_after_Hamming);


    position_in_archive = 0;
    unsigned position_in_file = 0;

    total.bit = new bool[2 * size_of_total];
    total.length = 2 * size_of_total;
    total.head = 0;
    total.tail = 0;

    data.bit = new bool[2 * size_of_data];
    data.length = 2 * size_of_data;
    data.head = 0;
    data.tail = 0;

    while (position_in_archive < size_of_file_after_Hamming && position_in_file < size_of_file) {

        while (total.size() < size_of_total && position_in_archive < size_of_file_after_Hamming) {
            archive.read((char *) &buf, sizeof(buf));
            position_in_archive++;
            for (uint8_t j = 128; j > 0; j >>= 1) {
                total.push((buf & j) / j);
            }
        }

        while (total.size() >= size_of_total) {
            Correction(total, size_of_total);
            for (int i = 0; i < size_of_total; ++i) {
                if (log2(i + 1) - unsigned(log2(i + 1)) != 0.0) {
                    data.push(total.dequeue());
                } else {
                    total.dequeue();
                }
            }
        }

        while (data.size() > 7 && position_in_file < size_of_file) {
            buf = 0;
            for (int j = 0; j < 8; j++) {
                buf = (buf << 1) + data.dequeue();
            }
            file.write((char *) &buf, sizeof(buf));
            position_in_file++;
        }
    }
    delete[] total.bit;
    delete[] data.bit;

}
