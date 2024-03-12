#pragma once
#include "Container.h"

class GroupContainer: public Container {
protected:
    size_t PearsonHashing(const void* elem, size_t elemSize, size_t containerSize);
public:
    GroupContainer(MemoryManager &mem): Container(mem) {}
};
