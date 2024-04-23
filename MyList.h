#pragma once
#include"ListAbstract.h"
#include"Mem.h"
#include"stdlib.h"

class List : public AbstractList
{
	struct ListElem//нужен конструктор для структуры (пока не нужно)
	{
		int* object;
		int obj_size;
		ListElem* next_ptr;
	};
	ListElem* list_head;

	class ListIterator : public Iterator
	{
	public:// остались тесты и эксепшены, динамик каст, итераторы
		ListElem* ptr;
		//проблема: можно получить доступ к вершине списка!

		ListIterator(ListElem* list_elem_ptr = NULL) { ptr = list_elem_ptr; } // +
		void* getElement(size_t& size); // +
		bool hasNext(); // +
		void goToNext(); // +
		bool equals(Iterator* right); // + спасёт ли меня dynamic cast от указателя НУЛЛ?
	};
public:
	List(MemoryManager& mem); // +
	~List(); // +

	int size(); // +
	size_t max_bytes(); // пока не ясно
	ListIterator* find(void* elem, size_t size); // ??
	ListIterator* newIterator(); // ??
	void remove(Iterator* iter); // +
	void clear(); // +
	bool empty(); // +

	int push_front(void* elem, size_t elemSize); // +
	void pop_front(); // +
	void* front(size_t& size); // +
	int insert(Iterator* iter, void* elem, size_t elemSize); // - work here
};