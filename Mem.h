#pragma once
#include "MemoryManager.h"

class Mem: public MemoryManager
{    
public:
    typedef struct mem_handle_t{
        int addr;
        int size;
        mem_handle_t(int addr, int size) :
        addr(addr),
        size(size)
        {}
        bool operator==(const mem_handle_t& other) { return other.addr == addr && other.size == size; }
        bool operator!=(const mem_handle_t& other) { return !operator==(other); }
    };
    Mem(size_t sz): MemoryManager(sz) {
        create(sz);
    }
    ~Mem(){
        destroy();
    }
    void* allocMem(size_t sz);
    bool freeMem(mem_handle_t h);
private:
    mem_handle_t get_block(int addr, int size);
    int get_max_block_size();
    int get_free_space();
    void print_blocks();
};