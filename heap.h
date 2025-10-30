//
// Created by Manju Muralidharan on 10/19/25.
//

#ifndef HEAP_H
#define HEAP_H

#include <iostream>
using namespace std;

struct MinHeap {
    int data[64];
    int size;

    MinHeap() { size = 0; }

    // Insert index and restore heap order
    void push(int idx, int weightArr[]) {
        if (size >= 64) {
            cerr << "Heap overflow: cannot push more elements.\n";
            return;
        }
        data[size] = idx;
        upheap(size, weightArr);
        size++;
    }

    // Remove and return smallest index
    int pop(int weightArr[]) {
        if (size == 0) {
            cerr << "Heap underflow: cannot pop from empty heap.\n";
            return -1;
        }
        int ret = data[0];
        size--;
        if (size > 0) {
            data[0] = data[size];
            downheap(0, weightArr);
        }
        return ret;
    }

    // Bubble up while child < parent
    void upheap(int pos, int weightArr[]) {
        while (pos > 0) {
            int parent = (pos - 1) / 2;
            if (weightArr[data[pos]] < weightArr[data[parent]]) {
                int tmp = data[pos];
                data[pos] = data[parent];
                data[parent] = tmp;
                pos = parent;
            } else {
                break;
            }
        }
    }

    // Push down while parent > smallest child
    void downheap(int pos, int weightArr[]) {
        while (true) {
            int left  = 2 * pos + 1;
            int right = 2 * pos + 2;
            int smallest = pos;

            if (left < size && weightArr[data[left]] < weightArr[data[smallest]]) {
                smallest = left;
            }
            if (right < size && weightArr[data[right]] < weightArr[data[smallest]]) {
                smallest = right;
            }
            if (smallest != pos) {
                int tmp = data[pos];
                data[pos] = data[smallest];
                data[smallest] = tmp;
                pos = smallest;
            } else {
                break;
            }
        }
    }
};

#endif
