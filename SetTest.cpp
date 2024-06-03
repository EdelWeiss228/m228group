#include "SetTest.h"

int SetTest::testInsert(size_t testCount) {
    chrono::high_resolution_clock::time_point start =
        chrono::high_resolution_clock::now();

    int err = 0;
    for (int_type i = 0; i < testCount; ++i) {
        err = set->insert(&i, sizeof(i));

        if (err == 2) {
            cout << "Element " << i << " was not inserted.\n";
            return 1;
        }
    }

    cout << "max list: " << set->longestList() << "\n";

    chrono::high_resolution_clock::time_point end =
        chrono::high_resolution_clock::now();
    double time_taken =
        chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    time_taken *= 1e-9;
    cout << "Time taken by insert() is : " << fixed << time_taken
         << setprecision(9) << " sec" << endl;

    return 0;
}

int SetTest::testFind(size_t sizeCount) {
    chrono::high_resolution_clock::time_point start =
        chrono::high_resolution_clock::now();

    Set::Iterator* it;

    for (int_type i = 0; i < sizeCount; ++i) {
        it = set->find(&i, sizeof(i));

        if (!it) {
            cout << "Element " << i << " was not found.\n";
            return 1;
        }

        delete it;
    }

    chrono::high_resolution_clock::time_point end =
        chrono::high_resolution_clock::now();
    double time_taken =
        chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    time_taken *= 1e-9;
    cout << "Time taken by find() is : " << fixed << time_taken
         << setprecision(9) << " sec" << endl;

    return 0;
}

int SetTest::removeOdd(size_t testCount) {
    chrono::high_resolution_clock::time_point start =
        chrono::high_resolution_clock::now();

    Set::Iterator* it = set->newIterator();

    int_type *elem, *elemCopy = (int_type*)malloc(sizeof(int_type));
    size_t size;

    while (it->hasNext()) {
        elem = (int_type*)(it->getElement(size));

        if ((*elem) % 2) {
            memcpy(elemCopy, elem, sizeof(int_type));
            set->remove(it);

            Set::Iterator* tmp = set->find(elemCopy, size);
            if (tmp) {
                cout << "Element " << (*elemCopy) << " was not deleted\n";
                return 1;
            }
        } else {
            it->goToNext();
        }
    }

    delete elemCopy;

    chrono::high_resolution_clock::time_point end =
        chrono::high_resolution_clock::now();
    double time_taken =
        chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    time_taken *= 1e-9;
    cout << "Time taken by removeOdd() is : " << fixed << time_taken
         << setprecision(9) << " sec" << endl;

    return 0;
}