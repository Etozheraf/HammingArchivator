#include <string>
#include <cmath>
#include "archiver.h"

void Correction(Queue &total, unsigned size_of_total) {
    unsigned index_of_error = 0;
    Queue control(uint32_t (log2(size_of_total + 1)) + 1);

    for (unsigned i = 0; i < log2(size_of_total + 1); ++i) {
        bool t = false;
        for (unsigned j = (1 << i); j < size_of_total; j++) {
            if (((j + 1) / (1 << i)) % 2 == 1) {
                t = t xor total.bit[(j + total.head) % total.length];
            }
        }
        control.push(t);
    }
    for (unsigned i = 0; i < control.size(); ++i) {
        if (control.bit[(i + control.head) % control.length] != total.bit[((1 << i) - 1 + total.head) % total.length]) {
            index_of_error += (1 << i);
        }
    }
    if (index_of_error > 0) {
        index_of_error--;
        total.bit[(index_of_error + total.head) % total.length] = !total.bit[(index_of_error + total.head) % total.length];
    }
    delete[] control.bit;
}

