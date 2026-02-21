
#include <iostream>
#include "LinkedList1.hpp"

using namespace std;

List::~List()
{
    this->clear();
}
// List::List(MemoryManager &mem) : AbstractList(mem){
// //     List* new_list = (List*)_memory.allocMem(sizeof(List));
// //     new_list->head = NULL;
// //     new_list->num_of_elems = 0;
//        this->head = NULL;
//         this->num_of_elems = 0;
//  }

void *List::ListIterator::getElement(size_t &size)
{
    if (!this->cur_node) return nullptr;
    size = this->cur_node->get_size();
    return this->cur_node->get_data();
}

bool List::ListIterator::hasNext()
{
    return this->cur_node != nullptr;
}

void List::ListIterator::goToNext()
{
    if (this->cur_node)
    {
        this->prev_node = cur_node;
        this->cur_node = this->cur_node->get_next();
    }
}

bool List::ListIterator::equals(Iterator *right)
{
    ListIterator *list_right = dynamic_cast<ListIterator *>(right);
    if (!list_right) return false;
    return this->cur_node == list_right->cur_node;
}

int List::push_front(void *elem, size_t elemSize)
{
    void *new_data = _memory.allocMem(elemSize);
    memcpy(new_data, elem, elemSize);
    ListNode *new_node = new ListNode(new_data, head, elemSize);
    this->head = new_node;
    num_of_elems++;
    return 0;
}

void List::pop_front()
{
    if (head)
    {
        ListNode *buf = head;
        head = head->get_next();
        _memory.freeMem(buf->get_data());
        delete buf;
        num_of_elems--;
    }
}

void *List::front(size_t &size)
{
    size = head->get_size();
    return head->get_data();
}

int List::insert(Iterator *iter, void *elem, size_t elemSize)
{
    ListIterator *list_iter = dynamic_cast<ListIterator *>(iter);
    if (list_iter && list_iter->prev_node)
    {
        num_of_elems++;
        void *new_data = _memory.allocMem(elemSize);
        memcpy(new_data, elem, elemSize);
        ListNode *new_elem = new ListNode(new_data, list_iter->cur_node, elemSize);
        list_iter->prev_node->change_next(new_elem);
        list_iter->cur_node = new_elem;
    }
    else
    {
        return push_front(elem, elemSize);
    }
    return 0;
}

size_t List::max_bytes()
{
    return 0;
}

int List::size()
{
    return num_of_elems;
}

Container::Iterator *List::newIterator()
{
    if (!head) return nullptr;
    return new ListIterator(head, NULL);
}

Container::Iterator *List::find(void *elem, size_t size)
{
    ListIterator *find_iter = dynamic_cast<ListIterator *>(newIterator());
    if (!find_iter) return NULL;
    while (find_iter->cur_node)
    {
        size_t cur_size = find_iter->cur_node->get_size();
        if (cur_size == size && memcmp(find_iter->cur_node->get_data(), elem, size) == 0)
        {
            return find_iter;
        }
        if (!find_iter->hasNext()) break;
        find_iter->goToNext();
    }
    delete find_iter;
    return NULL;
}

void List::remove(Iterator *iter)
{
    ListIterator *remove_iter = dynamic_cast<ListIterator *>(iter);
    if (!remove_iter || !remove_iter->cur_node) return;

    if (remove_iter->prev_node == NULL)
    {
        ListNode *buf = head;
        if (buf) {
            head = head->get_next();
            remove_iter->cur_node = head;
            
            _memory.freeMem(buf->get_data());
            delete buf;
            num_of_elems--;
        }
    }
    else
    {
        ListNode *buf = remove_iter->cur_node;
        if (buf) {
            remove_iter->prev_node->change_next(buf->get_next());
            remove_iter->cur_node = buf->get_next();
            
            _memory.freeMem(buf->get_data());
            delete buf;
            num_of_elems--;
        }
    }
}

void List::clear()
{
    while (num_of_elems != 0)
    {
        pop_front();
    }
}

bool List::empty()
{
    return !(bool)(num_of_elems);
}
