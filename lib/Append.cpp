#include <iostream>
#include <fstream>
#include <cmath>
#include <filesystem>
#include "archiver.h"


void Append(const std::string &archive_name, const std::string &filename_to_append) {
    std::ifstream file(filename_to_append, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << filename_to_append << " doesn't exist\n";
        file.close();
        exit(1);
    }

    std::ifstream in(archive_name, std::ios::binary);
    if (!in.is_open()) {
        std::cerr << archive_name << " doesn't exist\n";
        in.close();
        exit(1);
    }

    std::ofstream out(archive_name + "(append)", std::ios::binary);


    Queue total_in(97);
    Queue total_out(97);

    uint8_t buf = 0;

    ReadNBytes(in, total_in, 9);

    uint8_t ch[3];

    TransfusionAndGet3Chars(total_in, total_out, ch);

    if (ch[0] != 'H' || ch[1] != 'A' || ch[2] != 'F') {
        std::cerr << archive_name << " isn't an archive\n";
        in.close();
        out.close();
        std::filesystem::remove(archive_name + "(append)");
        exit(1);
    }

    WriteNBytes(out, total_out, 9);


    ReadNBytes(in, total_in, 12);
    unsigned size_of_control = 0;
    TransfusionAndGetUnsigned(total_in, total_out, size_of_control);
    WriteNBytes(out, total_out, 12);

    ReadNBytes(in, total_in, 12);
    unsigned size_of_header = 0;
    UnsignedFromQueue(total_in, size_of_header);
    size_of_header += filename_to_append.size() + 1 + 4;
    QueueFromUnsigned(total_out,size_of_header);
    WriteNBytes(out, total_out, 12);
    size_of_header -= filename_to_append.size() + 1 + 4;

    uint8_t char_for_null;
    std::string s;
    unsigned position_in_archive = in.tellg();
    unsigned offset_in;
    unsigned offset;
    while (position_in_archive < 3 * size_of_header) {
        s = "";
        char_for_null = 1;
        while (char_for_null != '\0') {
            ReadNBytes(in, total_in, 3);
            position_in_archive += 3;

            TransfusionAndGetChar(total_in, total_out, char_for_null);

            WriteNBytes(out, total_out, 3);

            s += char(char_for_null);
        }
        if (s == (filename_to_append + '\0')) {
            std::cerr << filename_to_append << " is already archived\n";
            in.close();
            out.close();
            std::filesystem::remove(archive_name + "(append)");
            exit(1);
        }

        offset = 0;

        position_in_archive += 12;
        ReadNBytes(in, total_in, 12);

        UnsignedFromQueue(total_in, offset);

        offset += filename_to_append.size() + 1 + 4;
        offset_in = offset;

        QueueFromUnsigned(total_out, offset);

        WriteNBytes(out, total_out, 12);
    }

    for (char k: filename_to_append) {
        QueueFromChar(total_out, k);

        WriteNBytes(out, total_out, 3);
    }
    QueueFromChar(total_out, '\0');

    WriteNBytes(out, total_out, 3);


    file.seekg(0, std::ios::end);
    uint32_t size_of_file = file.tellg();
    offset_in += size_of_file;

    file.seekg(0, std::ios::beg);
    QueueFromUnsigned(total_out, offset_in);

    WriteNBytes(out, total_out, 12);

    position_in_archive = in.tellg();
    in.seekg(0, std::ios::end);
    uint32_t size_of_archive = in.tellg();
    in.seekg(position_in_archive, std::ios::beg);

    delete[] total_in.bit;
    delete[] total_out.bit;

    uint32_t size_of_total = (1 << size_of_control) - 1;
    total_out.bit = new bool[2 * size_of_total + 1];
    total_out.head = 0;
    total_out.tail = 0;
    total_out.length = 2 * size_of_total + 1;

    total_in.bit = new bool[2 * size_of_total + 1];
    total_in.head = 0;
    total_in.tail = 0;
    total_in.length = 2 * size_of_total + 1;

    while (position_in_archive < size_of_archive) {

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


    uint32_t size_of_data = size_of_total - size_of_control;
    Queue data(2 * size_of_data + 1);

    uint32_t size_of_file_after_Hamming = ((size_of_file * 8 + size_of_data - 1) / size_of_data);
    size_of_file_after_Hamming = (size_of_file_after_Hamming * size_of_total + 7) / 8;

    uint32_t position_in_file = 0;
    position_in_archive = 0;

    while (position_in_file < size_of_file && position_in_archive < size_of_file_after_Hamming) {

        while (data.size() < size_of_data && position_in_file < size_of_file) {
            file.read((char *) &buf, sizeof(buf));
            position_in_file++;
            for (uint8_t j = 128; j > 0; j >>= 1) {
                data.push((buf & j) / j);
            }
        }

        if (position_in_file == size_of_file) {
            while (data.size() % size_of_data/* != 0*/) {
                data.push(false);
            }
        }

        while (data.size() >= size_of_data) {
            bool T[size_of_total];
            for (unsigned i = 0; i < size_of_total; ++i) {
                if ((log2(i + 1) - unsigned(log2(i + 1)) == 0.0)) {
                    T[i] = false;
                } else {
                    T[i] = data.dequeue();
                }
            }

            for (unsigned i = 0; i < size_of_control; ++i) {
                for (unsigned j = (1 << i); j < size_of_total; j++) {
                    if (((j + 1) / (1 << i)) % 2 == 1) {
                        T[(1 << i) - 1] = T[(1 << i) - 1] xor T[j];
                    }
                }
            }

            for (unsigned i = 0; i < size_of_total; ++i) {
                total_out.push(T[i]);
            }
        }

        while (total_out.size() > 7 && position_in_archive < size_of_file_after_Hamming - 1) {
            buf = 0;
            for (int j = 0; j < 8; j++) {
                buf = buf * 2 + total_out.dequeue();
            }
            out.write((char *) &buf, sizeof(buf));
            position_in_archive++;
        }

        if (position_in_archive == size_of_file_after_Hamming - 1) {
            while (total_out.size() % 8/* != 0*/) {
                total_out.push(false);
            }
            while (total_out.size() > 0) {
                buf = 0;
                for (int j = 0; j < 8; j++) {
                    buf = buf * 2 + total_out.dequeue();
                }
                out.write((char *) &buf, sizeof(buf));
                position_in_archive++;
            }
        }
    }
    delete[] data.bit;
    in.close();
    out.close();
    std::filesystem::remove(archive_name);
    std::filesystem::rename(archive_name + +"(append)", archive_name);
}