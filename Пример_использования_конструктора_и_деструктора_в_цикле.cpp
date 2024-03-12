#include<iostream>
using std::cout;
using std::endl;

class List {
	int i_;
public:
	List(int i):i_(i) {
		cout << "C-tor i = " << i << endl;
	}
	~List() {
		cout << "D-tor i = " << i_ << endl;
	}
};


int main() {

	List *arr;
	const int n = 10;
	arr = (List*)(malloc(sizeof(List) * n));
	for (int i = 0; i < n; ++i)
		new(&arr[i]) List(i);


	for (int i = 0; i < n; ++i)
		arr[i].~List();
	free(arr);
}