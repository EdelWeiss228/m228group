#pragma once
#include "Container.h"

// Базовый класс для однонаправленного и двунаправленного списков
class GroupList: public Container
{
public:
	GroupList(MemoryManager& mem) : Container(mem) { list_head = NULL; }

	struct ListElem
	{
		void* object;
		int obj_size;
		ListElem* next_ptr;
	};
	ListElem* list_head;

	class ListIterator : public Iterator
	{
	public:
		ListElem* prev_ptr;
		ListElem* ptr;

		ListIterator(ListElem* prev_elem_ptr = NULL, ListElem* list_elem_ptr = NULL)
		{ 
			prev_ptr = prev_elem_ptr;
			ptr = list_elem_ptr; 
		}
		void* getElement(size_t& size);
		bool hasNext();
		void goToNext();
		bool equals(Iterator* right);
	};

	Iterator* newIterator();
	void clear();
	bool empty();

	void* group_list_front(size_t& size);
};
