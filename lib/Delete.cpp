#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include "archiver.h"

void Delete(const std::string &archive_name, const std::string &filename_to_delete) {
    std::ifstream in(archive_name, std::ios::binary);
    if (!in.is_open()) {
        std::cerr << archive_name << " doesn't exist\n";
        exit(1);
    }
    std::ofstream out(archive_name + "(delete)", std::ios::binary);


    Queue total_in(97);

    uint8_t buf = 0;
    ReadNBytes(in, total_in, 9);

    uint8_t ch[3];
    for (uint8_t i = 0; i < 3; i++) {
        ch[i] = 1;
        for (int j = 0; j < 8; j++) {
            Correction(total_in, 3);
            ch[i] <<= 1;
            ch[i] += total_in.dequeue();
            total_in.dequeue();
            total_in.dequeue();
        }
    }
    if (ch[0] != 'H' || ch[1] != 'A' || ch[2] != 'F') {
        std::cerr << archive_name << " isn't an archive\n";
        in.close();
        out.close();
        std::filesystem::remove(archive_name + "(delete)");
        std::cerr << "this isn't an archive";
        exit(1);
    }

    ReadNBytes(in, total_in, 12);

    unsigned size_of_control = 0;
    UnsignedFromQueue(total_in, size_of_control);

    unsigned size_of_header = 0;
    ReadNBytes(in, total_in, 12);

    UnsignedFromQueue(total_in, size_of_header);

    uint8_t char_for_null;
    std::string s;
    bool file_exist = false;
    unsigned offset_of_file;
    unsigned offset_of_next_file = size_of_header;
    unsigned offset_of_file_after_Hamming = 3 * size_of_header;
    unsigned size_of_file;
    unsigned size_of_file_after_Hamming = 0;
    unsigned size_of_total = (1 << size_of_control) - 1;
    unsigned size_of_data = size_of_total - size_of_control;
    unsigned position_in_archive = in.tellg();
    unsigned index_of_file = 0;
    while (position_in_archive < 3 * size_of_header) {
        s = "";
        char_for_null = 1;
        while (char_for_null != '\0') {
            position_in_archive += 3;
            ReadNBytes(in, total_in, 3);

            CharFromQueue(total_in, char_for_null);

            s += char(char_for_null);
        }
        unsigned offset = 0;

        position_in_archive += 12;
        ReadNBytes(in, total_in, 12);

        UnsignedFromQueue(total_in, offset);

        offset_of_file = offset_of_next_file;
        offset_of_next_file = offset;
        size_of_file = offset_of_next_file - offset_of_file;
        size_of_file_after_Hamming = ((size_of_file * 8 + size_of_data - 1) / size_of_data);
        size_of_file_after_Hamming = (size_of_file_after_Hamming * size_of_total + 7) / 8;
        if (s == (filename_to_delete + '\0')) {
            file_exist = true;
            break;
        }
        offset_of_file_after_Hamming += size_of_file_after_Hamming;
        index_of_file++;
    }

    if (!file_exist) {
        //std::cerr << filename_to_delete << " is not in " << archive_name;
        in.close();
        out.close();
        std::filesystem::remove(archive_name + "(delete)");
        return;
        //exit(1);
    }

    in.seekg(0, std::ios::beg);


    Queue total_out(97);

    ReadNBytes(in, total_in, 9);

    bool t;
    TransfusionNChars(total_in, total_out, 3);

    WriteNBytes(out, total_out, 9);

    ReadNBytes(in, total_in, 12);

    TransfusionNChars(total_in, total_out, 4);

    WriteNBytes(out, total_out, 12);


    ReadNBytes(in, total_in, 12);

    while (total_in.size() > 0) {
        Correction(total_in, 3);
        total_in.dequeue();
        total_in.dequeue();
        total_in.dequeue();
    }

    size_of_header -= filename_to_delete.size() + 1 + 4;

    QueueFromUnsigned(total_out, size_of_header);

    WriteNBytes(out, total_out, 12);

    size_of_header += filename_to_delete.size() + 1 + 4;

    position_in_archive = in.tellg();
    unsigned offset;
    int index = 0;
    while (position_in_archive < 3 * size_of_header) {
        s = "";
        char_for_null = 1;
        if (index == index_of_file) {
            position_in_archive += 3 * (filename_to_delete.size() + 1 + 4);
            in.seekg(3 * (int64_t(filename_to_delete.size()) + 1 + 4), std::ios::cur);
            if (position_in_archive >= 3 * size_of_header) break;
        }
        while (char_for_null != '\0') {
            position_in_archive += 3;
            ReadNBytes(in, total_in, 3);

            TransfusionAndGetChar(total_in, total_out, char_for_null);

            WriteNBytes(out, total_out, 3);

            s += char(char_for_null);
        }

        offset = 0;
        position_in_archive += 12;
        ReadNBytes(in, total_in, 12);

        UnsignedFromQueue(total_in, offset);

        offset -= filename_to_delete.size() + 1 + 4;

        if (index >= index_of_file) {
            offset -= size_of_file;
        }
        index++;
        QueueFromUnsigned(total_out, offset);

        WriteNBytes(out, total_out, 12);
    }

    position_in_archive = in.tellg();
    in.seekg(0, std::ios::end);
    uint32_t size_of_archive = in.tellg();
    in.seekg(position_in_archive, std::ios::beg);

    delete[] total_in.bit;
    delete[] total_out.bit;


    total_out.bit = new bool[2 * size_of_total + 1];
    total_out.head = 0;
    total_out.tail = 0;
    total_out.length = 2 * size_of_total + 1;

    total_in.bit = new bool[2 * size_of_total + 1];
    total_in.head = 0;
    total_in.tail = 0;
    total_in.length = 2 * size_of_total + 1;


    while (position_in_archive < size_of_archive) {
        if (position_in_archive == offset_of_file_after_Hamming) {
            in.seekg(size_of_file_after_Hamming, std::ios::cur);
            position_in_archive += size_of_file_after_Hamming;
        }
        while (total_in.size() < size_of_total && position_in_archive < size_of_archive) {
            in.read((char *) &buf, sizeof(buf));
            position_in_archive++;
            for (uint8_t j = 128; j > 0; j >>= 1) {
                total_in.push((buf & j) / j);
            }
        }

        while (total_in.size() >= size_of_total) {
            Correction(total_in, size_of_total);
            for (int i = 0; i < size_of_total; ++i) {
                total_out.push(total_in.dequeue());
            }
        }

        if (position_in_archive == size_of_archive) {
            while (total_out.size() % 8) {
                total_out.push(false);
            }
        }

        while (total_out.size() > 7) {
            buf = 0;
            for (int j = 0; j < 8; j++) {
                buf = (buf << 1) + total_out.dequeue();
            }
            out.write((char *) &buf, sizeof(buf));
        }
    }

    delete[] total_in.bit;
    delete[] total_out.bit;
    in.close();
    out.close();
    std::filesystem::remove(archive_name);
    std::filesystem::rename(archive_name + +"(delete)", archive_name);
}
