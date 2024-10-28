#pragma once
#include <vector>
#include "MemoryManager.h"
#include "derevo.h"
#include <string>
using namespace std;


class TestTree {
private:
    MemoryManager* memory_manager;
    struct TestResult {
        bool passed;
        string cause;
    };

public:
    TestTree(MemoryManager& mem);
    void testTree1andTree2(int node_count1, int node_count2);
    void testNewIterator();
    void testGoToChild();
    void testGoToParent();
    void testHasNext();
    void testEquals();
    void testSize();
    void testEmpty();
    void testFind();
    void testInsert();
    void testRemove();
    void testClear();
    void testDifferentTypes();
    void testPerformance();

    Tree* makeTree1(int node_count);
private:
    Tree* makeTree2(int node_count);
    void printFail(string testName, string cause);
    TestResult testCompareTreeAndVector(Tree* tree, vector<int>& vector);
};
