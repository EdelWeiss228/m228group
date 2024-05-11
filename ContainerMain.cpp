#include"List_1.h"
#include "List_1_Test_class.h"

int main()
{
	Mem mm(0);
	List L(mm);
	List_1_Test_class test(&L);

	//std::cout << L.empty() << '\n';
	//int a = 5;
	//int* ptr1 = &a;
	//std::cout << L.push_front(ptr1, 4) << '\n';
	//int b = 7;
	//int* ptr2 = &b;
	//std::cout << L.push_front(ptr2, 4) << '\n';
	//int c = 9;
	//int* ptr3 = &c;
	//std::cout << L.push_front(ptr3, 4) << '\n';
	//std::cout << L.empty() << '\n';
	for (int i = 15; i > 0; i--)
	{
		L.push_front(&i, 4);
	}
	test.show_all_elem();
	//List::Iterator* newIter = L.newIterator();
	test.remove_every_n_elem(5);
	test.show_all_elem();
	// insert
	//for (int i = 300; i < 305; i++)
	//{
	//	//std::cout << L.insert(newIter, &i, 4) << '\n';
	//	L.insert(newIter, &i, 4);
	//}
	//std::cout << L.insert(newIter, &a, 4) << '\n';
	//L.remove(newIter);

	/*for (int i = 10; i > 0; i--)
	{
		L.pop_front();
	}*/

	//List::Iterator * newIter2 = L.newIterator();
	//for (int i = 0; i < 25; i++)
	//{
	//	//std::cout << *(int*)newIter2->getElement(bufer) << '\n';
	//	newIter2->goToNext();
	//}

	//find
	/*int b = 6;
	std::cout << L.find(&b, 4) << '\n';
	std::cout<< *(int*)newIter->getElement(bufer) << '\n';
	std::cout << L.size() << '\n';*/
	//mm.freeMem(newIter);
	//mm.freeMem(newIter2);
	return 0;
}