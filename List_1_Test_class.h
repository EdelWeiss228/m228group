#pragma once
#include "List_1.h"

class List_1_Test_class
{
	List* list;
public:
	List_1_Test_class(List* list_ptr) { list = list_ptr; }

	void show_all_elem();
	void remove_every_n_elem(int n);
	void insert_n_elem_to_m_position(int n, int m);
};