#include <cstdint>

class Queue {
public:
    Queue(uint32_t l) {
        length = l;
        bit = new bool[l];
        head = 0;
        tail = 0;
    }

    void push(bool b) {
        bit[tail] = b;
        if (tail == length - 1)
            tail = 0;
        else
            tail++;
    }

    bool dequeue() {
        bool x = bit[head];
        if (head == length - 1)
            head = 0;
        else
            head++;
        return x;
    }

    uint32_t size() {
        if (tail >= head)
            return tail - head;
        else
            return tail + length - head;
    }



    bool *bit;
    uint32_t head;
    uint32_t tail;
    uint32_t length;
};