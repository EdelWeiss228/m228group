#pragma once
#include "SetAbstract.h"
#include "List.h"
#include "iostream"

const size_t initCapacity = 1'000'000;

class Set: public AbstractSet {
private:
    size_t longestListSize;

    size_t m_capacity;
    size_t m_size;
    List** m_data;

    size_t findNextList(size_t startPos = 0);

    List::Iterator* newListIterator(size_t& listPos, bool toBegin);

    // void rehashing(size_t newCapacity);

    void cleanContainer();

public:
    size_t longestList() { return longestListSize; }

    Set(MemoryManager& mem): AbstractSet(mem), m_size(0), m_capacity(initCapacity) {
        m_data = (List**)_memory.allocMem(sizeof(List*) * m_capacity);
        cleanContainer();
    }

    ~Set() { clear(); _memory.freeMem(m_data); cout << "Set Destructor\n"; }

    class SetIterator: public AbstractSet::Iterator {
    private:
        Iterator* listIt;
        Set* set;
        size_t listPos;

    public:
        SetIterator(Set* set, Iterator* it, size_t listPos): listIt(it), set(set), listPos(listPos) {} 

        SetIterator(Set* set): set(set), listIt(set->newListIterator(listPos, true)) {}
            
        void* getElement(size_t& size);

        void goToNext();

        bool hasNext();
        bool equals(Iterator* right);

        friend class Set;

        ~SetIterator() { if (listIt) set->_memory.freeMem(listIt); }
    };

    size_t max_bytes();

    int size();
    int insert(void* elem, size_t size);

    Iterator* find(void* elem, size_t size);
    Iterator* newIterator();
    Iterator* end();

    void remove(Iterator* iter);
    void clear();
    bool empty();
};