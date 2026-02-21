                #pragma once

#include "derevo.h"
#include "Mem.h"

#include <limits>

struct TestData
{
    int i = 10;
    short c[2] = {7, 9};

    bool operator==(const TestData &data)
    {
        return (data.i == i) && (data.c[0] == c[0]) && (data.c[1] == c[1]);
    }

    bool operator!=(const TestData &data)
    {
        bool result = (*this == data);
        return (false == result);
    }
};

struct Test
{
    virtual ~Test() {}

    virtual void run() = 0;
    virtual bool result() = 0;

    Container &container()
    {
        return tree;
    }

    AbstractTree &abstractTree()
    {
        return tree;
    }

private:
    Mem mem{1024 * 1024 * 10}; // 10 MB pool for tests
    Tree tree{mem};
};

// ссылки в плюсах константные указатель
