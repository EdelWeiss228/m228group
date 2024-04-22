#pragma once
#include "MemoryManager.h"

class Mem: public MemoryManager
{    
public:
    size_t full_size = -1, real_size = 0, count = 0;
    typedef struct mem_handle{
        int addr;
        int size;
        mem_handle(int addr, int size) :
        addr(addr),
        size(size)
        {}
        bool operator==(const mem_handle& other) { return other.addr == addr && other.size == size; }
        bool operator!=(const mem_handle& other) { return !operator==(other); }
    };

    Mem(size_t sz): MemoryManager(sz) {full_size=sz;}

    ~Mem(){
    if (array_of_blocks != NULL){
        free(array_of_blocks);
        array_of_blocks = NULL;
        free(array_of_elements);
        array_of_elements = NULL;
    }
    full_size = -1;
    }
    virtual void* allocMem(size_t sz);
    virtual void freeMem(void* ptr);

private:
    virtual int get_max_block_size();
    virtual int get_free_space();
    virtual void print_blocks();
    mem_handle* array_of_blocks = NULL;
    char* array_of_elements = NULL;
};