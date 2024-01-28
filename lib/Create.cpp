#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>
#include "archiver.h"

void Create(const std::string &archive_name, std::vector<std::string> &filenames, unsigned size_of_control) {

    uint32_t offset = 0;
    offset += 3 + 4; // 3 to HAF 4 to control bits
    for (auto &filename: filenames) {
        offset += filename.size() + 1 + 4; // for name, '/0', offset
    }
    offset += 4; // for size of file in the end of header

    Queue data(8 * offset + 1);

    for (int j = 128; j > 0; j >>= 1) {
        data.push(('H' & j) / j);
    }
    for (int j = 128; j > 0; j >>= 1) {
        data.push(('A' & j) / j);
    }
    for (int j = 128; j > 0; j >>= 1) {
        data.push(('F' & j) / j);
    }
    for (uint32_t j = (UINT32_MAX >> 1) + 1; j > 0; j >>= 1) {
        data.push((size_of_control & j) / j);
    }

    std::vector<unsigned> size_of_file;
    for (auto &filename: filenames) {
        std::ifstream file(filename, std::ofstream::binary);

        if (!file.is_open()) {
            std::cerr << filename << " doesn't exist\n";
            continue;
        }

        for (uint32_t j = (UINT32_MAX >> 1) + 1; j > 0; j >>= 1) {
            data.push((offset & j) / j);
        }

        for (char k: filename) {
            for (int j = 128; j > 0; j >>= 1) {
                data.push((k & j) / j);
            }
        }
        for (int j = 128; j > 0; j >>= 1) {
            data.push(('\0' & j) / j);
        }

        file.seekg(0, std::ios::end);
        size_of_file.push_back(file.tellg());
        offset += file.tellg();
        file.close();
    }

    for (uint32_t j = (UINT32_MAX >> 1) + 1; j > 0; j >>= 1) {
        data.push((offset & j) / j); // size of archive before Hamming
    }

    Queue total(24);
    std::ofstream archive(archive_name, std::ofstream::binary);
    uint8_t buf;
    while (0 < data.size()) {
        bool b;
        for (uint32_t j = 0; j < 8; j++) {
            b = data.dequeue();
            total.push(b);
            total.push(b);
            total.push(b);
        }

        WriteNBytes(archive, total, 3);
    }

    delete[] data.bit;
    delete[] total.bit;

    uint32_t size_of_total = (1 << size_of_control) - 1;
    total.bit = new bool[2 * size_of_total];
    total.head = 0;
    total.tail = 0;
    total.length = 2 * size_of_total;

    uint32_t size_of_data = size_of_total - size_of_control;
    data.bit = new bool[2 * size_of_data];
    data.head = 0;
    data.tail = 0;
    data.length = 2 * size_of_data;

    while (!size_of_file.empty()) {
        std::ifstream file(filenames.front(), std::ofstream::binary);

        uint32_t size_of_file_after_Hamming = ((size_of_file.front() * 8 + size_of_data - 1) / size_of_data);
        size_of_file_after_Hamming = (size_of_file_after_Hamming * size_of_total + 7) / 8;

        uint32_t position_in_file = 0;
        uint32_t position_in_archive = 0;

        while (position_in_file < size_of_file.front() && position_in_archive < size_of_file_after_Hamming) {

            while (data.size() < size_of_data && position_in_file < size_of_file.front()) {
                file.read((char *) &buf, sizeof(buf));
                position_in_file++;
                for (uint8_t j = 128; j > 0; j >>= 1) {
                    data.push((buf & j) / j);
                }
            }

            if (position_in_file == size_of_file.front()) {
                while (data.size() % size_of_data/* != 0*/) {
                    data.push(false);
                }
            }

            while (data.size() >= size_of_data) {
                bool t[size_of_total];
                for (unsigned i = 0; i < size_of_total; ++i) {
                    if ((log2(i + 1) - unsigned(log2(i + 1)) == 0.0)) {
                        t[i] = false;
                    } else {
                        t[i] = data.dequeue();
                    }
                }

                for (unsigned i = 0; i < size_of_control; ++i) {
                    for (unsigned j = (1 << i); j < size_of_total; j++) {
                        if (((j + 1) / (1 << i)) % 2 == 1) {
                            t[(1 << i) - 1] = t[(1 << i) - 1] xor t[j];
                        }
                    }
                }

                for (unsigned i = 0; i < size_of_total; ++i) {
                    total.push(t[i]);
                }
            }

            while (total.size() > 7 && position_in_archive < size_of_file_after_Hamming - 1) {
                buf = 0;
                for (int j = 0; j < 8; j++) {
                    buf = buf * 2 + total.dequeue();
                }
                archive.write((char *) &buf, sizeof(buf));
                position_in_archive++;
            }

            if (position_in_archive == size_of_file_after_Hamming - 1) {
                while (total.size() % 8/* != 0*/) {
                    total.push(false);
                }
                while (total.size() > 0) {
                    buf = 0;
                    for (int j = 0; j < 8; j++) {
                        buf = buf * 2 + total.dequeue();
                    }
                    archive.write((char *) &buf, sizeof(buf));
                    position_in_archive++;
                }
            }
        }

        size_of_file.erase(size_of_file.begin());
        filenames.erase(filenames.begin());
    }
}

