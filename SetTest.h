#pragma once
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>

#include "Mem.h"
#include "Set.h"

using namespace std;

#define int_type int

class SetTest {
   private:
    Set* set;

   public:
    SetTest(Set* set) : set(set) {}
    ~SetTest() { delete set; }

    int testInsert(size_t testCount);

    int testFind(size_t testCount);

    int removeOdd(size_t testCount);
};