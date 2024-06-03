#include <iostream>

#include "Mem.h"
#include "Set.h"
#include "SetTest.h"

// g++ -std=c++17 -o out GroupContainer.cpp GroupList.cpp List.cpp Set.cpp
// SetTest.cpp SetTestMain.cpp g++ SetTestMain.cpp SetTest.cpp Set.cpp
// GroupContainer.cpp -o a -std=c++17 leaks --atExit -- ./a
// g++ List_1.cpp GroupList.cpp GroupContainer.cpp Set.cpp SetTest.cpp
// SetTestMain.cpp -o out -std=c++17
//  valgrind --leak-check=yes myprog arg1 arg2

int main() {
    Mem mm(0);

    SetTest Test(new Set(mm));
    size_t size = 1000000;

    Test.testInsert(size);
    Test.testFind(size);
    Test.removeOdd(size);

    return 0;
}