#include "Mem.h"
#include <iostream>

void* Mem::allocMem(size_t block_size) {
    return new char[block_size];
}

void Mem::freeMem(void* ptr) {
    delete[] (char*)ptr;
}
