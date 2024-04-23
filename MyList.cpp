#include"MyList.h"

List::List(MemoryManager& mem) : AbstractList(mem)
{
	list_head = NULL;
}

List::~List()
{
	if (list_head) List::clear();
}

int List::size()
{
	int counter = 0;
	if (list_head)
	{
		ListElem* current_object = list_head;
		do
		{
			counter++;
			current_object = current_object->next_ptr;
		} while (current_object);
	}
	return counter;
}

size_t List::max_bytes()
{
	return 0;
}

List::ListIterator* List::find(void* elem, size_t size)
{
	if (list_head && elem && size > 0)
	{
		ListIterator* iter_ptr = (ListIterator*)List::_memory.allocMem(sizeof(ListIterator));
		if (iter_ptr)
		{
			ListElem* current_elem = list_head;
			do
			{
				if (size == current_elem->obj_size &&
					!memcmp(elem, current_elem->object, size))
				{
					(*iter_ptr).List::ListIterator::ListIterator(current_elem);
					return iter_ptr;
				}
				current_elem = current_elem->next_ptr;
			} while (current_elem);
		}
		else
		{
			//exception
		}
	}
	return NULL;
}

List::ListIterator* List::newIterator()
{
	if (list_head)
	{
		ListIterator* iter_ptr = (ListIterator*)List::_memory.allocMem(sizeof(ListIterator));
		if (iter_ptr)
		{
			iter_ptr->List::ListIterator::ListIterator(list_head);
			return iter_ptr;
		}
		else
		{
			//exception
		}
	}
	return NULL;
}

void List::remove(Iterator* iter)
{
	ListIterator* iterator = dynamic_cast<ListIterator*>(iter);
	if (iterator)
	{
		ListIterator* current_elem = List::newIterator();
		if (current_elem)
		{
			if (iterator->equals(current_elem))	pop_front();
			else
			{
				ListIterator* previous_elem = List::newIterator();
				if (previous_elem)
				{
					do
					{
						current_elem->goToNext();
						if (current_elem->equals(iterator))
						{
							List::_memory.freeMem(current_elem->ptr->object);
							iterator->goToNext();
							previous_elem->ptr->next_ptr = iterator->ptr;
							List::_memory.freeMem(current_elem->ptr);
						}
						else
						{
							previous_elem->goToNext();
							current_elem->goToNext();
						}
					} while (current_elem->ptr->next_ptr);
					List::_memory.freeMem(previous_elem);
				}
				else
				{
					// exception
				}
			}
			List::_memory.freeMem(current_elem);
		}
		else
		{
			// exception
		}
	}
	else
	{
		// exception
	}
}

void List::clear()
{
	if (list_head)
	{
		ListElem* current_elem = list_head;
		do
		{
			List::_memory.freeMem(current_elem->object);
			current_elem = current_elem->next_ptr;
		} while (current_elem);
		list_head = NULL;
	}
	else
	{
		//exception
	}
}

bool List::empty()
{
	if (list_head)
		return false;
	return true;
}

void* List::ListIterator::getElement(size_t& size)
{
	if (ptr && ptr->object)
	{
		size = ptr->obj_size;
		return ptr->object;
	}
	return NULL;
}

bool List::ListIterator::hasNext()
{
	if (ptr->next_ptr)
		return true;
	return false;
}

void List::ListIterator::goToNext()
{
	if (ptr)
	{
		ptr = ptr->next_ptr;
	}
	else
	{
		//exception
	}
}

bool List::ListIterator::equals(Iterator* right)
{
	ListIterator* checking_iter = dynamic_cast<ListIterator*>(right);
	if (checking_iter)
	{
		if (ptr == NULL || checking_iter->ptr == NULL) return ptr == checking_iter->ptr;
		if (ptr->obj_size == checking_iter->ptr->obj_size &&
			!memcmp(ptr->object, checking_iter->ptr->object, ptr->obj_size)) return  true;
		else return false;
	}
	else
	{
		// exception
	}
}

int List::push_front(void* elem, size_t elemSize)
{
	if (!list_head)//если контейнер пуст
	{
		if (elemSize > 0 && elem)
		{
			//не забыть исправить тип
			list_head = (ListElem*)List::_memory.allocMem(sizeof(ListElem));
			if (list_head)
			{
				list_head->object = (int*)List::_memory.allocMem(sizeof(int));
				if (list_head->object)
				{
					list_head->object = (int*)memcpy(list_head->object, elem, 4);
					list_head->next_ptr = NULL;
					list_head->obj_size = elemSize;
					return 0;
				}
			}
		}
	}
	else// если в нём есть элементы
	{
		ListElem* new_node = (ListElem*)List::_memory.allocMem(sizeof(ListElem));
		if (new_node)
		{
			new_node->object = (int*)List::_memory.allocMem(sizeof(int));
			if (new_node->object &&
				elemSize > 0 && elem)
			{
				new_node->object = (int*)memcpy(new_node->object, elem, 4);
				new_node->next_ptr = list_head;
				new_node->obj_size = elemSize;
				list_head = new_node;
				new_node = NULL;
				return 0;
			}
		}

	}
	return 1;
}

void List::pop_front()
{
	if (list_head)
	{
		List::_memory.freeMem(list_head->object);
		ListElem* bufer;
		bufer = list_head->next_ptr;
		List::_memory.freeMem(list_head);
		list_head = bufer;
		bufer = NULL;
	}
	else
	{
		//exception;
	}
}

void* List::front(size_t& size)
{
	if (list_head->object)
	{
		size = list_head->obj_size;
		return list_head;
	}
	return NULL;
}

int List::insert(Iterator* iter, void* elem, size_t elemSize)
{
	ListIterator* iterator = dynamic_cast<ListIterator*>(iter);
	if (iterator)
	{
		if (iterator->ptr)
		{
			ListIterator* current_elem = List::newIterator();
			if (current_elem)
			{
				if (iterator->equals(current_elem))// если добавляем в начало списка
				{
					return push_front(elem, elemSize);
				}
				else// если добавляем не в начало
				{
					ListIterator* previous_elem = List::newIterator();
					if (previous_elem)
					{
						current_elem->goToNext();
						do
						{
							if (current_elem->equals(iterator))
							{
								ListElem* new_node = (ListElem*)List::_memory.allocMem(sizeof(ListElem));
								if (new_node &&
									elemSize > 0 && elem)
								{
									new_node->object = (int*)List::_memory.allocMem(sizeof(int));
									if (new_node->object)
									{
										new_node->object = (int*)memcpy(new_node->object, elem, 4);
										new_node->obj_size = elemSize;
										new_node->next_ptr = current_elem->ptr;
										previous_elem->ptr->next_ptr = new_node;
										List::_memory.freeMem(current_elem);
										List::_memory.freeMem(previous_elem);
										return 0;
									}
									else
									{
										List::_memory.freeMem(previous_elem);
										List::_memory.freeMem(current_elem);
										return 1;
									}
								}
								else
								{
									List::_memory.freeMem(previous_elem);
									List::_memory.freeMem(current_elem);
									return 1;
								}
							}
							else
							{
								previous_elem->goToNext();
								current_elem->goToNext();
							}
						} while (current_elem->ptr->next_ptr);
						List::_memory.freeMem(previous_elem);
					}
				}
				List::_memory.freeMem(current_elem);
			}
		}
		return 1;
	}
	else
	{
		return 1;
	}
}
