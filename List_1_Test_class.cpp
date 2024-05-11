#include "List_1_Test_class.h"

void List_1_Test_class::show_all_elem()
{
	List::Iterator* new_iterator = list->newIterator(); size_t bufer;
	List::ListIterator* iter = dynamic_cast<List::ListIterator*>(new_iterator);
	if (iter)
	{
		do
		{
			std::cout << *(int*)iter->getElement(bufer) << ' ';
			iter->goToNext();
		} while (iter->ptr);
		std::cout << '\n';
		free(iter);
	}
	else std::cout << "\nDynamic cast error or list is empty!\n";
}

void List_1_Test_class::remove_every_n_elem(int n)//work here;
{
	if (n > 0)
	{
		List::Iterator* new_iterator = list->newIterator();
		List::ListIterator* iter = dynamic_cast<List::ListIterator*>(new_iterator);
		if (iter)
		{
			int counter = 0;
			do
			{
				counter++;
				if (counter == n)
				{
					list->remove(iter);
					counter = 0;
				}
				else iter->goToNext();
			} while (iter->ptr);
			free(iter);
		}
		else std::cout << "\nDynamic cast error!\n";
	}
}

void List_1_Test_class::insert_n_elem_to_m_position(int n, int m)
{

}