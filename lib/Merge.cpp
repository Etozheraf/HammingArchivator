#include <iostream>
#include <string>
#include <fstream>
#include "archiver.h"

void Merge(const std::string &archive_result_name, const std::string &archive1_name, const std::string &archive2_name) {
    std::ifstream archive1(archive1_name, std::ios::binary);
    if (!archive1.is_open()) {
        std::cerr << archive1_name << " doesn't exists\n";
        archive1.close();
        exit(1);
    }

    std::ifstream archive2(archive2_name, std::ios::binary);
    if (!archive2.is_open()) {
        std::cerr << archive2_name << " doesn't exists\n";
        archive1.close();
        exit(1);
    }


    Queue total_in(97);

    uint8_t buf = 0;
    uint8_t ch[3];

    ReadNBytes(archive1, total_in, 9);

    for (uint32_t i = 0; i < 3; i++) {
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
        std::cerr << archive1_name << " isn't an archive\n";
        archive1.close();
        archive2.close();
        exit(1);
    }

    ReadNBytes(archive1, total_in, 12);

    unsigned size_of_control = 0;
    UnsignedFromQueue(total_in, size_of_control);


    ReadNBytes(archive2, total_in, 9);

    for (uint32_t i = 0; i < 3; i++) {
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
        std::cerr << archive2_name << " isn't an archive\n";
        archive1.close();
        archive2.close();
        exit(1);
    }

    ReadNBytes(archive2, total_in, 12);

    unsigned tmp = 0;
    UnsignedFromQueue(total_in, tmp);

    if (tmp != size_of_control) {
        std::cerr << "Archives cannot be merged\n";
        archive1.close();
        archive2.close();
        exit(1);
    }

    std::vector<std::string> filenames;

    ReadNBytes(archive1, total_in, 12);

    unsigned size_of_header1 = 0;
    UnsignedFromQueue(total_in, size_of_header1);


    archive2.close();

    uint8_t char_for_null;
    std::string s;
    unsigned position_in_archive = archive1.tellg();
    while (position_in_archive < 3 * size_of_header1) {
        s = "";
        char_for_null = 1;
        while (char_for_null != '\0') {
            position_in_archive += 3;
            ReadNBytes(archive1, total_in, 3);

            CharFromQueue(total_in, char_for_null);
            if (char_for_null != '\0') s += char(char_for_null);
        }
        Delete(archive2_name, s);

        for (int i = 0; i < 12; ++i) {
            archive1.read((char *) &buf, sizeof(uint8_t));
            position_in_archive++;
        }
    }

    archive1.close();

    archive1.open(archive1_name, std::ios::binary);
    archive2.open(archive2_name, std::ios::binary);
    std::ofstream result(archive_result_name, std::ios::binary);

    Queue total_out(97);

    for (int i = 0; i < 9; ++i) {
        archive2.read((char *) &buf, sizeof(buf));
    }
    ReadNBytes(archive1, total_in, 9);

    TransfusionAndGet3Chars(total_in, total_out, ch);

    WriteNBytes(result, total_out, 9);

    for (int i = 0; i < 12; ++i) {
        archive2.read((char *) &buf, sizeof(uint8_t));
    }
    ReadNBytes(archive1, total_in, 12);

    TransfusionAndGetUnsigned(total_in, total_out, size_of_control);

    WriteNBytes(result, total_out, 12);

    for (int i = 0; i < 12; ++i) {
        archive1.read((char *) &buf, sizeof(uint8_t));
    }
    ReadNBytes(archive2, total_in, 12);

    unsigned size_of_header2 = 0;
    UnsignedFromQueue(total_in, size_of_header2);

    size_of_header2 -= 11;

    size_of_header1 += size_of_header2;

    QueueFromUnsigned(total_out, size_of_header1);

    WriteNBytes(result, total_out, 12);
    size_of_header1 -= size_of_header2;

    position_in_archive = archive1.tellg();

    unsigned offset1;
    while (position_in_archive < 3 * size_of_header1) {
        s = "";
        char_for_null = 1;
        while (char_for_null != '\0') {
            position_in_archive += 3;
            ReadNBytes(archive1, total_in, 3);

            TransfusionAndGetChar(total_in, total_out, char_for_null);

            WriteNBytes(result, total_out, 3);

            s += char(char_for_null);
        }

        position_in_archive += 12;
        ReadNBytes(archive1, total_in, 12);

        offset1 = 0;
        UnsignedFromQueue(total_in, offset1);

        offset1 += size_of_header2;

        QueueFromUnsigned(total_out, offset1);

        WriteNBytes(result, total_out, 12);
        offset1 -= size_of_header2;
    }

    size_of_header2 += 11;

    position_in_archive = archive2.tellg();
    unsigned offset2;
    while (position_in_archive < 3 * size_of_header2) {
        s = "";
        char_for_null = 1;
        while (char_for_null != '\0') {
            position_in_archive += 3;
            ReadNBytes(archive2, total_in, 3);

            TransfusionAndGetChar(total_in, total_out, char_for_null);

            WriteNBytes(result, total_out, 3);

            s += char(char_for_null);
        }

        position_in_archive+= 12;
        ReadNBytes(archive2, total_in, 12);

        offset2 = 0;
        UnsignedFromQueue(total_in, offset2);

        offset2 -= 11;
        offset2 += offset1;

        QueueFromUnsigned(total_out, offset2);


        WriteNBytes(result, total_out, 12);
    }

    delete[] total_in.bit;
    delete[] total_out.bit;

    position_in_archive = archive1.tellg();
    archive1.seekg(0, std::ios::end);
    uint32_t size_of_archive = archive1.tellg();
    archive1.seekg(position_in_archive, std::ios::beg);

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
            archive1.read((char *) &buf, sizeof(buf));
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
            result.write((char *) &buf, sizeof(buf));
        }
    }

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


    position_in_archive = archive2.tellg();
    archive2.seekg(0, std::ios::end);
    size_of_archive = archive2.tellg();
    archive2.seekg(position_in_archive, std::ios::beg);

    while (position_in_archive < size_of_archive) {

        while (total_in.size() < size_of_total && position_in_archive < size_of_archive) {
            archive2.read((char *) &buf, sizeof(buf));
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
            result.write((char *) &buf, sizeof(buf));
        }
    }

    archive1.close();
    archive2.close();
    result.close();
}
