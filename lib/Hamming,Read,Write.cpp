#include "archiver.h"
#include <fstream>

void ReadNBytes(std::ifstream &in, Queue &total_in, const uint32_t& N) {
    uint8_t buf = 0;
    for (int i = 0; i < N; ++i) {
        in.read((char *) &buf, sizeof(buf));
        for (uint8_t j = 128; j > 0; j >>= 1) {
            total_in.push((buf & j) / j);
        }
    }
}

void WriteNBytes(std::ofstream &out, Queue &total_out, const uint32_t& N) {
    uint8_t buf = 0;
    for (uint32_t i = 0; i < N; ++i) {
        buf = 0;
        for (uint32_t j = 0; j < 8; j++) {
            buf = buf * 2 + total_out.dequeue();
        }
        out.write((char *) &buf, sizeof(buf));
    }
}

void TransfusionAndGet3Chars(Queue &total_in, Queue &total_out, uint8_t ch[]) {
    bool t;
    for (uint32_t i = 0; i < 3; i++) {
        ch[i] = 1;
        for (int j = 0; j < 8; j++) {
            Correction(total_in, 3);
            ch[i] <<= 1;
            t = total_in.dequeue();
            ch[i] += t;
            total_out.push(t);
            total_out.push(total_in.dequeue());
            total_out.push(total_in.dequeue());
        }
    }
}

void TransfusionAndGetUnsigned(Queue &total_in, Queue &total_out, uint32_t &ui) {
    bool t;
    while (total_in.size() > 0) {
        Correction(total_in, 3);
        ui <<= 1;
        t = total_in.dequeue();
        ui += t;
        total_out.push(t);
        total_out.push(total_in.dequeue());
        total_out.push(total_in.dequeue());
    }
}

void TransfusionAndGetChar(Queue &total_in, Queue &total_out, uint8_t &ch) {
    bool t;
    ch = 1;
    for (int j = 0; j < 8; j++) {
        Correction(total_in, 3);
        ch <<= 1;
        t = total_in.dequeue();
        ch += t;
        total_out.push(t);
        total_out.push(total_in.dequeue());
        total_out.push(total_in.dequeue());
    }
}

void TransfusionNChars(Queue &total_in, Queue &total_out,const uint32_t& N) {
    for (uint32_t i = 0; i < N; i++) {
        for (int j = 0; j < 8; j++) {
            Correction(total_in, 3);
            total_out.push(total_in.dequeue());
            total_out.push(total_in.dequeue());
            total_out.push(total_in.dequeue());
        }
    }
}

void UnsignedFromQueue(Queue &total_in, uint32_t &ui) {
    while (total_in.size() > 0) {
        Correction(total_in, 3);
        ui <<= 1;
        ui += total_in.dequeue();
        total_in.dequeue();
        total_in.dequeue();
    }
}

void QueueFromUnsigned(Queue &total_out, uint32_t &ui) {
    bool t;
    for (uint32_t j = (UINT32_MAX >> 1) + 1; j > 0; j >>= 1) {
        t = (ui & j) / j;
        total_out.push(t);
        total_out.push(t);
        total_out.push(t);
    }
}

void CharFromQueue(Queue &total_in, uint8_t &ch) {
    while (total_in.size() > 0) {
        Correction(total_in, 3);
        ch <<= 1;
        ch += total_in.dequeue();
        total_in.dequeue();
        total_in.dequeue();
    }
}

void QueueFromChar(Queue &total_out, const char &ch) {
    bool t;
    for (uint8_t j = 128; j > 0; j >>= 1) {
        t = (ch & j) / j;
        total_out.push(t);
        total_out.push(t);
        total_out.push(t);
    }
}

