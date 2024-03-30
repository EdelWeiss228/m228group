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
    Mem(size_t sz): MemoryManager(sz) {}
    ~Mem(){}
    virtual void* allocMem(size_t sz);
    virtual void freeMem(void* ptr);
    bool operator=(const int& addr){}
private:
    virtual mem_handle_t get_block(int addr, int size);
    virtual int get_max_block_size();
    virtual int get_free_space();
    virtual void print_blocks();
};