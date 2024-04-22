#pragma once
#include "SetAbstract.h"
#include "List.h"
#include <list>

const size_t initCapacity = 1000;

class Set: public AbstractSet {
private:
    size_t m_capacity;
    size_t m_size;
    List** m_data;

    size_t findNextList(size_t startPos = 0);

    List::Iterator* newListIterator(size_t& index, bool toBegin);

    void clearContainer();

public:
    Set(MemoryManager& mem): AbstractSet(mem),
    m_size(0),
    m_capacity(initCapacity) {
        m_data = (List**)_memory.allocMem(sizeof(List*) * m_capacity);
        clearContainer();
    }

    ~Set() { clear(); _memory.freeMem(m_data); }

    class SetIterator: public AbstractSet::Iterator {
    private:
        Iterator* listIt;
        Set* set;
        size_t listPos;

    public:
        SetIterator(Set* set, size_t listPos, bool toBegin = true):
            set(set),
            listPos(listPos),
            listIt(set->newListIterator(listPos, toBegin)) {}

        void* getElement(size_t& size);

        void goToNext();

        bool hasNext();
        bool equals(Iterator* right);

        friend class Set;
    };

    size_t max_bytes();

    int size();
    int insert(void* elem, size_t size);

    Iterator* find(void* elem, size_t size);
    Iterator* newIterator();

    void remove(Iterator* iter);
    void clear();
    bool empty();
};