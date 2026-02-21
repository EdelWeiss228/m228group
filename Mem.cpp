#include "Mem.h"
#include <iostream>
#include <cstdlib>

struct BlockHeader {
    size_t size;  // Total size (header + data)
    bool isUsed;
};

Mem::Mem(size_t sz) : MemoryManager(sz) {
    _storage = std::malloc(sz);
    if (_storage) {
        BlockHeader* head = (BlockHeader*)_storage;
        head->size = sz;
        head->isUsed = false;
    }
}

Mem::~Mem() {
    std::free(_storage);
}

size_t Mem::maxBytes() {
    return size();
}

void* Mem::allocMem(size_t block_size) {
    if (!_storage) return nullptr;

    // Data alignment (8 bytes)
    size_t aligned_data_size = block_size;
    if (aligned_data_size % 8 != 0) aligned_data_size += (8 - (aligned_data_size % 8));
    
    size_t total_needed = aligned_data_size + sizeof(BlockHeader);

    BlockHeader* current = (BlockHeader*)_storage;
    while ((char*)current < (char*)_storage + size()) {
        if (!current->isUsed && current->size >= total_needed) {
            // Can we split? Minimum block size to split: total_needed + header + some data
            if (current->size >= total_needed + sizeof(BlockHeader) + 8) {
                size_t remaining_size = current->size - total_needed;
                current->size = total_needed;
                
                BlockHeader* next = (BlockHeader*)((char*)current + total_needed);
                next->size = remaining_size;
                next->isUsed = false;
            }
            current->isUsed = true;
            return (void*)((char*)current + sizeof(BlockHeader));
        }
        current = (BlockHeader*)((char*)current + current->size);
    }
    return nullptr;
}

void Mem::freeMem(void* ptr) {
    if (!ptr || (char*)ptr < (char*)_storage || (char*)ptr >= (char*)_storage + size()) return;

    BlockHeader* header = (BlockHeader*)((char*)ptr - sizeof(BlockHeader));
    header->isUsed = false;

    // Robust coalescing: traverse and merge all adjacent free blocks
    BlockHeader* current = (BlockHeader*)_storage;
    while ((char*)current < (char*)_storage + size()) {
        BlockHeader* next = (BlockHeader*)((char*)current + current->size);
        if ((char*)next < (char*)_storage + size() && !current->isUsed && !next->isUsed) {
            current->size += next->size;
            // Don't advance 'current', it might merge with the next 'next'
        } else {
            current = next;
        }
    }
}
