#include "test.h"
#include "MemoryManager.h"
#include <chrono>
#include <iostream>

TestTree::TestTree(MemoryManager& mem) : memory_manager(&mem) { }

void TestTree::testTree1andTree2(int node_count1, int node_count2) 
{
    cout << "testTree1andTree2 started" << endl;
    bool result=true;
    Tree* tree1 = makeTree1(node_count1);
    vector<int> vector1(node_count1 + 1);
    vector1[0] = 0;
    for (int index = 0; index < node_count1; ++index)
        vector1[index + 1] = node_count1 - 1 - index;
    TestResult testResult = this->testCompareTreeAndVector(tree1, vector1);
    if (!testResult.passed) 
    {
        this->printFail("Tree1 isn`t correct", testResult.cause);
        result = false;
    }
    if (result) 
        cout << "Tree1 is correct" << endl;
    delete tree1;
    result = true;
    Tree* tree2 = this->makeTree2(node_count2);
    vector<int> vector2(node_count2 * node_count2 + node_count2 + 1);
    for (int index = 0; index < node_count2 * node_count2 + node_count2 + 1;++index) 
        vector2[index]=index;
    testResult = this->testCompareTreeAndVector(tree2, vector2);
    if (!testResult.passed) 
    {
        this->printFail("Tree2 isn`t correct", testResult.cause);
        result = false;
    }
    if (result) 
        cout << "Tree2 is correct" << endl;
    delete tree2;
    cout << "testTree1andTree2 finished" << endl << endl;
}

void TestTree::testNewIterator() 
{
    cout << "testNewIterator started" << endl;
    bool result=true;
    size_t size;
    Tree* tree = new Tree(*this->memory_manager);
    Tree::Iterator* iterator = tree->newIterator();
    if (iterator != nullptr) 
    {
        this->printFail("newIterator, test-1", "iteator!=nullptr but expected nullptr");
        result = false;
    }
    delete iterator;
    delete tree;
    if (result) 
        cout << "newIterator, test-1 passed" << endl;
    result = true;
    int node_count1 = 100;
    Tree* tree1 = this->makeTree1(node_count1);
    Tree::Iterator* iterator1 = tree1->newIterator();
    int* value = static_cast<int*>(iterator1->getElement(size));
    if (value != nullptr)
    {
        if (*value != 0)
        {
            this->printFail("newIterator, test-2", "value=" + to_string(*value) + "but expected value=0");
            result = false;
        }
    }
    else
    {
        this->printFail("newIterator, test-2", "getElement(size)=nullptr but expected getElement(size)!=nullptr");
        result = false;
    }
    delete iterator1;
    delete tree1;
    if (result) 
        cout << "newIterator, test-2 passed" << endl;
    cout << "testNewIterator finished" << endl << endl;
}

void TestTree::testGoToChild() 
{
    cout << "testGoToChild started" << endl;
    bool result=true;
    int node_count1 = 100;
    Tree* tree1 = this->makeTree1(node_count1);
    Tree::Iterator* iterator1 = tree1->newIterator();
    if (iterator1->goToChild(0)) 
    {
        for (int i = 0; i < 5; i++)
            iterator1->goToNext();
        size_t size;
        int* value = static_cast<int*>(iterator1->getElement(size));
        if (*value != 94) {
            this->printFail("goToChild, test-1","value=" + to_string(*value) + " but expected value=94");
            result = false;
        }
    }
    else 
    {
        this->printFail("goToChild, test-1", "goToChild(0)=false but expected goToChild(0)=true");
        result = false;
    }
    delete iterator1;
    delete tree1;
    if (result) 
        cout << "goToChid, test-1: passed" << endl;
    cout << "testGoToChild finished" << endl << endl;
}

void TestTree::testGoToParent() 
{
    cout << "testGoToParent started" << endl;
    bool result=true;
    int node_count1 = 100;
    Tree* tree1 = this->makeTree1(node_count1);
    Tree::Iterator* iterator1 = tree1->newIterator();
    if (iterator1->goToParent()) 
    {
        this->printFail("goToParent, test-1", "goToParent()=true but expected goToParent()=false");
        result = false;
    }
    delete iterator1;
    delete tree1;
    if (result) 
        cout << "goToParent, test-1: passed" << endl;
    cout << "testGoToParent finished" << endl << endl;
}

void TestTree::testHasNext() 
{
    cout << "testHasNext started" << endl;
    bool result=true;
    int node_count1 = 1000;
    Tree* tree1 = this->makeTree1(node_count1);
    Tree::Iterator* iterator1 = tree1->newIterator();
    if (!iterator1->hasNext()) 
    {
        this->printFail("hasNext, test-1", "hasNext()=false but expected hasNext()=true");
        result = false;
    }
    delete iterator1;
    if (result) 
        cout << "hasNext, test-1: passed" << endl;
    result = true;
    iterator1 = tree1->newIterator();
    iterator1->goToChild(0);
    for (int i = 0; i < 250; ++i)
        iterator1->goToNext();
    if (!iterator1->hasNext()) 
    {
        this->printFail("hasNext, test-2", "hasNext()=false expected hasNext()=true");
        result = false;
    }
    delete iterator1;
    if (result) 
        cout << "hasNext, test-2: passed" << endl;
    result = true;
    iterator1 = tree1->newIterator();
    iterator1->goToChild(0);
    for (int i = 1; i < node_count1; i++)
        iterator1->goToNext();
    if (iterator1->hasNext()) 
    {
        this->printFail("hasNext, test-3", "hasNext()=true expected hasNext()=false");
        result = false;
    }
    if (result) 
        cout << "hasNext, test-3: passed" << endl;
    delete iterator1;
    delete tree1;
    cout << "testHasNext finished" << endl << endl;
}

void TestTree::testEquals() 
{
    cout << "testEquals started" << endl;
    bool result=true;
    int node_count1 = 100;
    Tree* tree1 = this->makeTree1(node_count1);
    Tree::Iterator* iterator1 = tree1->newIterator();
    Tree::Iterator* iterator2 = tree1->newIterator();
    if (!iterator1->equals(iterator2)) 
    {
        this->printFail(
            "equals, test-1",
            "equals(iterator2)=false but expected equals(iterator2)=true");
        result = false;
    }
    delete iterator1;
    delete iterator2;
    if (result) 
        cout << "equals, test-1: passed" << endl;
    result = true;
    iterator1 = tree1->newIterator();
    iterator1->goToChild(0);
    for (int i = 0; i < 13; i++)
        iterator1->goToNext();
    iterator2 = tree1->newIterator();
    iterator2->goToChild(0);
    for (int i = 0; i < 7; i++)
        iterator2->goToNext();
    if (iterator1->equals(iterator2)) 
    {
        this->printFail("equals, test-2", "equals(iterator2)=true but expected equals(iterator2)=false");
        result = false;
    }
    delete iterator1;
    delete iterator2;
    if (result) 
        cout << "equals, test-2: passed" << endl;
    delete tree1;
    cout << "testEquals finished" << endl << endl;
}

void TestTree::testSize() 
{
    std::cout << "testSize started" << std::endl;
    bool result=true;
    Tree* tree = new Tree(*this->memory_manager);
    if (tree->size() != 0) 
    {
        this->printFail("size, test-1", "size()=" + to_string(tree->size()) + "but expected size()=0");
        result = false;
    }
    delete tree;
    if (result) 
        cout << "size, test-1: passed" << endl;
    result = true;
    int node_count1 = 100;
    Tree* tree1 = this->makeTree1(node_count1);
    if (tree1->size() != 101) 
    {
        this->printFail("size, test-2", "size()=" + std::to_string(tree1->size()) + " but expected size()=101");
        result = false;
    }
    delete tree1;
    if (result) 
        cout << "size, test-2: passed" << endl;
    result = true;
    int node_count2 = 10;
    Tree* tree2 = this->makeTree2(node_count2);
    if (tree2->size() != 111) 
    {
        this->printFail("size, test-3", "size()=" + std::to_string(tree2->size()) + " but expected size()=111");
        result = false;
    }
    if (result) 
        cout << "size, test-3: passed" << endl;
    delete tree2;
    cout << "testSize finished" << endl << endl;
}

void TestTree::testEmpty() 
{
    cout << "testEmpty started" << endl;
    bool result = true;
    Tree* tree1 = new Tree(*this->memory_manager);
    if (!tree1->empty()) 
    {
        this->printFail("empty, test-1",
            "empty()=false but expected empty()=true");
        result = false;
    }
    delete tree1;
    if (result) 
        cout << "empty, test-1: passed" << endl;
    result = true;
    int node_count1 = 100;
    tree1 = this->makeTree1(node_count1);
    if (tree1->empty()) 
    {
        this->printFail("empty, test-2",
            "empty()=true but expected empty()=false");
        result = false;
    }
    delete tree1;
    if (result) 
        cout << "empty, test-2: passed" << endl;
    cout << "testEmpty finished" << endl << endl;
}

void TestTree::testFind()
{
    cout << "testFind started" << endl;
    bool result=true;
    int node_count1 = 100;
    Tree* tree1 = makeTree1(node_count1);
    size_t size;
    int value = 200;
    Tree::Iterator* iterator1 = tree1->find(&value, sizeof(value));
    if (iterator1 != nullptr) 
    {
        this->printFail("find, test-1", "find(200, sizeof(int))!=nullptr but "
            "find(200, sizeof(int))==nullptr");
        result = false;
    }
    delete iterator1;
    if (result) 
        cout << "find, test-1: passed" << endl;
    result = true;
    value = 10;
    iterator1 = tree1->find(&value, sizeof(value));
    if (iterator1 == nullptr)
    {
        this->printFail("find, test-2", "find(10, sizeof(int))!=nullptr but find(10, sizeof(int))==nullptr");
        result = false;
    }
    else if (*static_cast<int*>(iterator1->getElement(size)) != 10) 
    {
        this->printFail("find, test-2", "found element contain" + to_string(*static_cast<int*>(iterator1->getElement(size))) + " but it must contain 10");
        result = false;
    }
    delete iterator1;
    if (result) 
        cout << "find, test-2: passed" << endl;
    delete tree1;
    cout << "testFind finished" << endl << endl;
}

void TestTree::testInsert()
{
    cout << "testInsert started" << endl;
    bool result=true;
    int node_count1 = 100;
    Tree* tree1 = this->makeTree1(node_count1);
    Tree::Iterator* iterator1 = tree1->newIterator();
    int elem = 0;
    if (tree1->insert(iterator1, 0, &elem, sizeof(elem)) == 1) 
    {
        this->printFail("insert, test-1", "insert() returned 1 but expected 0");
        result = false;
    }
    delete iterator1;
    delete tree1;
    if (result) 
        cout << "insert, test-1: passed" << endl;
    result = true;
    tree1 = this->makeTree1(100);
    iterator1 = tree1->newIterator();
    elem = -1;
    tree1->insert(iterator1, 0, &elem, sizeof(elem));
    elem = 0;
    iterator1->goToChild(0);
    tree1->insert(iterator1, 0, &elem, sizeof(elem));
    for (int i = 0; i < 100; i++)
        iterator1->goToNext();
    tree1->insert(iterator1, 0, &elem, sizeof(elem));
    vector<int> test_vector(104);
    test_vector[0] = 0;
    test_vector[1] = -1;
    test_vector[2] = 0;
    for (int index = 3; index < 103; ++index) 
    {
        test_vector[index] = 102 - index;
    }
    test_vector[103] = 0;
    TestResult testResult = this->testCompareTreeAndVector(tree1, test_vector);
    if (!testResult.passed) 
    {
        this->printFail("insert, test-2", testResult.cause);
        result = false;
    }
    delete iterator1;
    if (result) 
        cout << "insert, test-2: passed" << endl;
    delete tree1;
    cout << "testInsert finished" << endl << endl;
}

void TestTree::testRemove() 
{
    cout << "testRemove started" << endl;
    bool result=true;
    int node_count1 = 100;
    Tree* tree1 = this->makeTree1(node_count1);
    Tree::Iterator* iterator1 = tree1->newIterator();
    if (tree1->remove(iterator1, 1)) 
    {
        this->printFail("remove, test-1","remove()=true but expected remove()=false");
        result = false;
    }
    if (result) 
        cout << "remove, test-1: passed" << endl;
    delete iterator1;
    delete tree1;
    result = true;
    tree1 = this->makeTree1(node_count1);
    iterator1 = tree1->newIterator();
    iterator1->goToChild(0);
    if (!tree1->remove(iterator1, 1)) 
    {
        this->printFail("remove, test-2","remove()=false but expected remove()=true");
        result = false;
    }
    if (result) 
        cout << "remove, test-2: passed" << endl;
    delete iterator1;
    delete tree1;

    result = true;
    int node_count2 = 10;
    Tree* tree2 = this->makeTree2(node_count2);
    Tree::Iterator* iterator2 = tree2->newIterator();
    iterator2->goToChild(0);
    tree2->remove(iterator2);
    vector<int> vector2(node_count2 * node_count2);
    vector2[0] = 0;
    for (int index = 1; index < node_count2 * node_count2; ++index) 
        vector2[index] = node_count2 + 1 + index;
    TestResult testResult = this->testCompareTreeAndVector(tree2, vector2);
    if (!testResult.passed) 
    {
        this->printFail("remove, test-3", testResult.cause);
        result = false;
    }
    delete iterator2;
    if (result) 
        cout << "remove, test-3: passed" << endl;
    delete tree2;
    cout << "testRemove finished" << endl << endl;
}

void TestTree::testClear() 
{
    cout << "testClear started" << endl;
    bool result;
    result = true;
    int node_count1 = 100;
    Tree* tree1 = makeTree1(node_count1);
    tree1->clear();
    if (!tree1->empty()) 
    {
        this->printFail("clear, test-1", "empty()=false but empty()=true");
        result = false;
    }
    if (result) 
        cout << "clear, test-1: passed" << endl;
    delete tree1;
    cout << "testClear finished" << endl << endl;
}

TestTree::TestResult TestTree::testCompareTreeAndVector(Tree* tree, vector<int>& vector) 
{
    Tree::Iterator* iterator = tree->newIterator();
    int* value;
    size_t size;
    TestResult result = { true, "" };
    if (tree->size() == vector.size())
    {
        for (int index = 0; index < vector.size(); ++index) 
        {
            value = static_cast<int*>(iterator->getElement(size));
            if (value == nullptr) 
            {
                result.passed = false;
                result.cause = "testCompareTreeAndVector: index=" + to_string(index) + ", getElement(size)==nullptr but expected getElement(size)!==nullptr";
                break;
            }
            if (size != sizeof(int)) 
            {
                result.passed = false;
                result.cause = "testCompareTreeAndVector: index=" + to_string(index) + ", element isn`t int";
                break;
            }
            if (*value != vector[index]) 
            {
                result.passed = false;
                result.cause ="testCompareTreeAndVector: index=" + to_string(index) +", vector[index]=" + to_string(vector[index]) + " value=" + to_string(*value) +" but expected they are equal";
                break;
            }
            /*if (!iterator->goToChild(0))
            {
                if (iterator->hasNext())
                    iterator->goToNext();
                else
                {
                    iterator->goToParent();
                    iterator->goToNext();
                }
            }*/
            iterator->goToNext();
        }
    }
    else 
    {
        result.passed = false;
        result.cause = "testCompareTreeAndVector: tree->size()=" +to_string(tree->size()) + ", vector.get_size()=" + to_string(vector.size()) + " but expected vector.get_size()==tree->size()";
    }
    delete iterator;
    return result;
}

void TestTree::testDifferentTypes() 
{
    cout << "testDifferentTypes started" << endl;
    struct TestStruct
    {
        int element1 = 0;
        int element2 = 0;
    } testStruct;
    
    Tree* tree = new Tree(*this->memory_manager);
    Tree::Iterator* iterator = tree->newIterator();
    int index = 0;
    tree->insert(iterator, 0, &index, sizeof(index));
    delete iterator;
    iterator = tree->newIterator();
    for (index = 0; index < 50; ++index) 
    {
        tree->insert(iterator, 0, &index, sizeof(index));
        testStruct.element1 = index;
        testStruct.element2 = index + 1;
        tree->insert(iterator, 0, &testStruct, sizeof(testStruct));
    }
    delete iterator;
    testStruct.element1 = 12;
    testStruct.element2 = 13;
    iterator = tree->find(&testStruct, sizeof(testStruct));
    bool result = true;
    size_t size;
    if (iterator != nullptr) 
    {
        testStruct = *static_cast<TestStruct*>(iterator->getElement(size));
        if (testStruct.element1 != 12 || testStruct.element2 != 13) 
        {
            this->printFail("testDifferentTypes, test-1", "find() isn`t working correctly");
            result = false;
        }
    }
    else 
    {
        this->printFail("testDifferentTypes, test-1", "find() return nullptr");
        result = false;
    }
    if (result) 
        cout << "testDifferentTypes, test-1: passed" << endl;
    delete iterator;

    result = true;
    iterator = tree->newIterator();
    iterator->goToChild(0);
    for (int i = 0; i < 13; i++)
        iterator->goToNext();
    int value = *static_cast<int*>(iterator->getElement(size));
    if (value != 43 || size != sizeof(int)) 
    {
        this->printFail("testDifferentTypes, test-2", "getElement() return incorrect values");
        result = false;
    }
    if (result) 
        cout << "testDifferentTypes, test-2: passed" << endl;
    delete iterator;
    result = true;
    iterator = tree->newIterator();
    iterator->goToChild(0);
    testStruct = *static_cast<TestStruct*>(iterator->getElement(size));
    if (testStruct.element1 != 49 || testStruct.element2 != 50 || size != sizeof(testStruct)) 
    {
        this->printFail("testDifferentTypes, test-3", "getElement() return incorrect values");
        result = false;
    }
    if (result)
        cout << "testDifferentTypes, test-3: passed" << endl;
    delete iterator;
    delete tree;
    cout << "testDifferentTypes finished" << endl << endl;
}

void TestTree::testPerformance() {
    cout << "Performance tests:" << endl;

    auto start = chrono::system_clock::now();
    Tree* tree1 = this->makeTree1(1000000);
    auto end = chrono::system_clock::now();
    chrono::duration<double> duration = end - start;
    cout << "Performance test 1: " << duration.count() << endl;

    start = chrono::system_clock::now();
    delete tree1;
    end = chrono::system_clock::now();
    duration = end - start;
    cout << "Performance test 2: " << duration.count() << endl;

    start = chrono::system_clock::now();
    Tree* tree2 = this->makeTree2(1000);
    end = chrono::system_clock::now();
    duration = end - start;
    cout << "Performance test 3: " << duration.count() << endl;

    start = chrono::system_clock::now();
    delete tree2;
    end = chrono::system_clock::now();
    duration = end - start;
    cout << "Performance test 4: " << duration.count() << endl;

    Tree* tree = new Tree(*this->memory_manager);
    Tree::Iterator* iterator = tree->newIterator();
    int index = 0;
    tree->insert(iterator, 0, &index, sizeof(index));
    delete iterator;
    start = chrono::system_clock::now();
    iterator = tree->newIterator();
    for (index = 1; index <= 1000; ++index) 
    {
        if (tree->insert(iterator, 0, &index, sizeof(index)) == 1) 
            throw "Performance test failed";
        iterator->goToChild(0);
    }
    end = chrono::system_clock::now();
    duration = end - start;
    delete iterator;
    delete tree;
    cout << "Performance test 5: " << duration.count() << endl << endl;
    return;
}

Tree* TestTree::makeTree1(int node_count) {
    Tree* tree1 = new Tree(*this->memory_manager);
    int index = 0;
    tree1->insert(nullptr, 0, &index, sizeof(index));
    Tree::Iterator* iterator1 = tree1->newIterator();
    for (index = 0; index < node_count; ++index) 
        tree1->insert(iterator1, 0, &index, sizeof(index));
    delete iterator1;
    return tree1;
}

Tree* TestTree::makeTree2(int node_count) {
    Tree* tree2 = new Tree(*this->memory_manager);
    //Tree::Iterator* iterator2 = tree2->newIterator();
    int value = 0;
    tree2->insert(nullptr, 0, &value, sizeof(value));
    //delete iterator2;

    Tree::Iterator* iterator2 = tree2->newIterator();
    for (int index1 = 0; index1 < node_count; ++index1) 
    {
        value = (node_count - index1) * node_count - index1;
        tree2->insert(iterator2, 0, &value, sizeof(value));
        iterator2->goToChild(0);
        value += node_count+1;
        for (int index2 = 0; index2 < node_count; ++index2) 
        {
            value--; 
            tree2->insert(iterator2, 0, &value, sizeof(value));
        }
        iterator2->goToParent();
    }
    delete iterator2;
    return tree2;
}

void TestTree::printFail(const std::string testName, const std::string cause) 
{
    cout << testName << ": failed, " << cause << endl;
}