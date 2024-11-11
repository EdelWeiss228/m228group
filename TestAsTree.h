#pragma once

#include "TestData.h"

struct TestAsTree: public Test
{
    void run() override;

    bool result() override;

private:
    bool insert();
    bool multiinsert();
    bool remove();

    bool _testResult = true;
};
