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

void Set::SetIterator::goToNext() {
    if (!listIt) return;
    if (listIt->hasNext()) listIt->goToNext();

    listPos = set->findNextList(listPos + 1);

    set->_memory.freeMem(listIt);
    listIt = listPos >= set->m_capacity ? nullptr : set->m_data[listPos]->newIterator();
}

bool Set::SetIterator::hasNext() { 
    if (!listIt) return false;
    if (listIt->hasNext()) return true;

    return set->findNextList(listPos + 1) < set->m_capacity;
}

bool Set::SetIterator::equals(Iterator* right) {
    SetIterator* rightIt = dynamic_cast<SetIterator*>(right);
    return listIt->equals(rightIt->listIt);
}

int Set::size() { return m_size; }

size_t Set::max_bytes() { return _memory.size(); }

Container::Iterator* Set::find(void* elem, size_t size) {
    size_t hash = PearsonHashing(elem, size, m_capacity);

    if (!m_data[hash] || m_data[hash]->empty()) return nullptr;

    Iterator *listIt = m_data[hash]->find(elem, size);
    SetIterator* it = new SetIterator(this, hash, false);

    while (!listIt->equals(it->listIt)) it->listIt->goToNext();

    return it;
}

Set::Iterator* Set::newIterator() {
    return new SetIterator(this, 0);
}

void Set::remove(Iterator* iter) {
    SetIterator* it = dynamic_cast<SetIterator*>(iter);

    size_t listSize = m_data[it->listPos]->size();

    m_data[it->listPos]->remove(it->listIt);

    if (listSize == 1) it->goToNext();
    if (listSize > 0) m_size--;
    
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