#include"List_1.h"

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

GroupList::Iterator* List::find(void* elem, size_t size)
{
	if (list_head && elem && size > 0)
	{
		ListIterator* iter_ptr = (ListIterator*)List::_memory.allocMem(sizeof(ListIterator));
		if (iter_ptr)
		{
			ListElem* prev_elem = nullptr;
			ListElem* current_elem = list_head;
			do
			{
				if (size == current_elem->obj_size &&
					!memcmp(elem, current_elem->object, size))
				{
					ListIterator new_iter(prev_elem, current_elem);
					ListIterator* new_iter_ptr = &new_iter;
					iter_ptr = (ListIterator*)memcpy(iter_ptr, new_iter_ptr, sizeof(ListIterator));
					return iter_ptr;
				}
				prev_elem = current_elem;
				current_elem = current_elem->next_ptr;
			} while (current_elem);
			List::_memory.freeMem(iter_ptr);
		}
		else
		{
			Error err("Memory for iterator wasn`t allocated!");
			throw err;
		}
	}
	return NULL;
}

//void List::remove(Iterator* iter)
//{
//	ListIterator* iterator = dynamic_cast<ListIterator*>(iter);
//	if (iterator)
//	{
//		Iterator* bufer = List::newIterator();
//		ListIterator* current_elem = dynamic_cast<ListIterator*>(bufer);
//		if (current_elem)
//		{
//			if (iterator->equals(current_elem))
//			{
//				iterator->goToNext();
//				pop_front();
//			}
//			else
//			{
//				bufer = List::newIterator();
//				ListIterator* previous_elem = dynamic_cast<ListIterator*>(bufer);
//				if (previous_elem)
//				{
//					current_elem->goToNext();
//					do
//					{
//						if (current_elem->equals(iterator))
//						{
//							List::_memory.freeMem(current_elem->ptr->object);
//							iterator->goToNext();
//							previous_elem->ptr->next_ptr = iterator->ptr;
//							List::_memory.freeMem(current_elem->ptr);
//							break;
//						}
//						else
//						{
//							previous_elem->goToNext();
//							current_elem->goToNext();
//						}
//					} while (current_elem->ptr);
//					List::_memory.freeMem(previous_elem);
//				}
//				else
//				{
//					Error err("Dynamic cast error!");
//					throw err;
//				}
//			}
//			List::_memory.freeMem(current_elem);
//		}
//		else
//		{
//			Error err("Dynamic cast error or list is empty!");
//			throw err;
//		}
//	}
//	else
//	{
//		Error err("Dynamic cast error or iterator was NULL!");
//		throw err;
//	}
//}

//void List::remove(Iterator* iter)
//{
//	ListIterator* iterator = dynamic_cast<ListIterator*>(iter);
//	if (iterator)
//	{
//		void* scouting_elem = iterator->ptr->object;
//		void* current_elem_obj = list_head->object;
//		ListElem* curr_elem_next_ptr = list_head->next_ptr;
//
//		if (scouting_elem == current_elem_obj)
//		{
//			iterator->goToNext();
//			pop_front();
//		}
//		else
//		{
//			ListElem* previous_elem_ptr = list_head;
//
//			current_elem_obj = curr_elem_next_ptr->object;
//			curr_elem_next_ptr = curr_elem_next_ptr->next_ptr;
//			do
//			{
//				if (scouting_elem == current_elem_obj)
//				{
//					List::_memory.freeMem(previous_elem_ptr->next_ptr->object);
//					iterator->goToNext();
//					List::_memory.freeMem(previous_elem_ptr->next_ptr);
//					previous_elem_ptr->next_ptr = iterator->ptr;
//					return;
//				}
//				else
//				{
//					current_elem_obj = curr_elem_next_ptr->object;
//					curr_elem_next_ptr = curr_elem_next_ptr->next_ptr;
//					previous_elem_ptr = previous_elem_ptr->next_ptr;
//				}
//			} while (curr_elem_next_ptr);
//
//			if (scouting_elem == current_elem_obj)
//			{
//				List::_memory.freeMem(previous_elem_ptr->next_ptr->object);
//				iterator->goToNext();
//				List::_memory.freeMem(previous_elem_ptr->next_ptr);
//				previous_elem_ptr->next_ptr = iterator->ptr;
//				return;
//			}
//		}
//	}
//	else
//	{
//		Error err("Dynamic cast error or iterator was NULL!");
//		throw err;
//	}
//}

void List::remove(Iterator* iter)
{
	ListIterator* iterator = dynamic_cast<ListIterator*>(iter);
	if (iterator)
	{
		if (iterator->prev_ptr)
		{
			List::_memory.freeMem(iterator->ptr->object);
			iterator->prev_ptr->next_ptr = iterator->ptr->next_ptr;
			List::_memory.freeMem(iterator->ptr);
			iterator->ptr = iterator->prev_ptr->next_ptr;
		}
		else
		{
			iterator->ptr = iterator->ptr->next_ptr;
			pop_front();
		}
	}
	else
	{
		Error err("Dynamic cast error or iterator was NULL!");
		throw err;
	}
}

int List::push_front(void* elem, size_t elemSize)
{
	if (!list_head)//если контейнер пуст
	{
		if (elemSize > 0 && elem)
		{
			list_head = (ListElem*)List::_memory.allocMem(sizeof(ListElem));
			if (list_head)
			{
				list_head->object = List::_memory.allocMem(elemSize);
				if (list_head->object)
				{
					list_head->object = memcpy(list_head->object, elem, elemSize);
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
			new_node->object = List::_memory.allocMem(elemSize);
			if (new_node->object &&
				elemSize > 0 && elem)
			{
				new_node->object = memcpy(new_node->object, elem, elemSize);
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
		Error err("Container is empty!");
		throw err;
	}
}

void* List::front(size_t& size)
{
	return group_list_front(size);
}

int List::insert(Iterator* iter, void* elem, size_t elemSize)
{
	ListIterator* iterator = dynamic_cast<ListIterator*>(iter);
	if (iterator)
	{
		if (iterator->ptr)
		{
			Iterator* bufer = List::newIterator();
			ListIterator* current_elem = dynamic_cast<ListIterator*>(bufer);
			if (current_elem)
			{
				if (iterator->equals(current_elem))// если добавляем в начало списка
				{
					return push_front(elem, elemSize);
				}
				else// если добавляем не в начало
				{
					bufer = List::newIterator();
					ListIterator* previous_elem = dynamic_cast<ListIterator*>(bufer);
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
									new_node->object = List::_memory.allocMem(elemSize);
									if (new_node->object)
									{
										new_node->object = memcpy(new_node->object, elem, elemSize);
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
						} while (current_elem->ptr);
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

//int List::insert(Iterator* iter, void* elem, size_t elemSize)
//{
//	ListIterator* iterator = dynamic_cast<ListIterator*>(iter);
//	if (iterator && iterator->ptr)
//	{
//		ListElem* curr_elem = list_head;
//		if (iterator->ptr == curr_elem) return push_front(elem, elemSize);// если добавляем в начало списка
//		else// если добавляем не в начало
//		{
//			curr_elem = curr_elem->next_ptr;
//			do
//			{
//				if (iterator->ptr == curr_elem)
//				{
//					ListElem* new_node = (ListElem*)List::_memory.allocMem(sizeof(ListElem));
//					if (new_node)
//					{
//						if (elemSize > 0 && elem)
//						{
//							new_node->object = memcpy(new_node->object, elem, elemSize);
//							new_node->obj_size = elemSize;
//							new_node->next_ptr = iterator->ptr;
//							iterator->prev_ptr->next_ptr = new_node;
//							return 0;
//						}
//						List::_memory.freeMem(new_node);
//						return 1;
//					}
//					else
//					{
//						return 1;
//					}
//				}
//				else
//				{
//					curr_elem = curr_elem->next_ptr;
//				}
//			} while (curr_elem->next_ptr);
//		}
//	}
//	else
//	{
//		return 1;
//	}
//}