#pragma once
#include "MemoryManager.h"

// Простейший менеджер памяти, использует ::new и ::delete
class Mem: public MemoryManager
{
    void* _storage;
public:
    Mem(size_t sz);
    ~Mem() override;
    
    void* allocMem(size_t sz) override;
    void freeMem(void* ptr) override;
    size_t maxBytes() override;
};
