#include "Set.h"

size_t Set::findNextList(size_t startPos) {
    int i = startPos;

    for (; i < m_capacity; ++i)
        if (m_data[i] && !m_data[i]->empty()) break;

    return i;
}

List::Iterator* Set::newListIterator(size_t& index, bool toBegin) {
    if (!toBegin) return m_data[index]->newIterator();

    index = findNextList();
    
    return (index >= m_capacity) ? nullptr : m_data[index]->newIterator();
}

void Set::clearContainer() { for (size_t i = 0; i < m_capacity; ++i) m_data[i] = NULL; }

void* Set::SetIterator::getElement(size_t& size) {
    return listIt ? listIt->getElement(size) : nullptr;
}


Set::Iterator* Set::newIterator() {
    return new SetIterator(this, 0);
}


void Set::clear() {
    for (int i = 0; i < m_capacity; ++i)
        if (m_data[i]) m_data[i]->clear();

    m_size = 0;
}

bool Set::empty() { return m_size == 0; }

int Set::insert(void* elem, size_t size) {
    size_t hash = PearsonHashing(elem, size, m_capacity);

    // printf("inserting with hash %llu\n", hash);
    if (m_data[hash] && m_data[hash]->find(elem, size))
        return 1;

    if (!m_data[hash])
        m_data[hash] = new List(_memory);

    int error = m_data[hash]->push_front(elem, size);
    if (!error) m_size++;

    return error;
}