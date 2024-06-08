#include <iostream>
#include "derevo.h"  // Include the Tree class definition
#include "Mem.h"     // Include the Mem class definition

using namespace std;



void assertEqual(bool condition, const char* message) {
    if (condition) {
        cout << "PASS: " << message << endl;
    } else {
        cout << "FAIL: " << message << endl;
    }
}

void testInsertAndFind() {
    Mem mem(1000);
    Tree tree(mem);

    int value1 = 10;
    int value2 = 20;
    size_t size;

    Tree::Iterator* rootIter = tree.newIterator();
    tree.insert(rootIter, 0, &value1, sizeof(value1));
    Tree::Iterator* iter = tree.find(&value1, sizeof(value1));
    assertEqual(iter != nullptr, "Insert and Find value1");

    tree.insert(iter, 0, &value2, sizeof(value2));
    Tree::Iterator* iter2 = tree.find(&value2, sizeof(value2));
    assertEqual(iter2 != nullptr, "Insert and Find value2");

    delete iter;
    delete iter2;
    delete rootIter;
}

void testRemoveLeafNode() {
    Mem mem(1000);
    Tree tree(mem);

    int value1 = 10;
    int value2 = 20;

    Tree::Iterator* rootIter = tree.newIterator();
    tree.insert(rootIter, 0, &value1, sizeof(value1));
    Tree::Iterator* iter = tree.find(&value1, sizeof(value1));
    tree.insert(iter, 0, &value2, sizeof(value2));

    Tree::Iterator* iter2 = tree.find(&value2, sizeof(value2));
    assertEqual(iter2 != nullptr, "Find value2 before removal");

    assertEqual(tree.remove(iter2, 1), "Remove leaf node");
    assertEqual(tree.find(&value2, sizeof(value2)) == nullptr, "Find value2 after removal");

    delete iter;
    delete iter2;
    delete rootIter;
}

void testRemoveNonLeafNode() {
    Mem mem(1000);
    Tree tree(mem);

    int value1 = 10;
    int value2 = 20;

    Tree::Iterator* rootIter = tree.newIterator();
    tree.insert(rootIter, 0, &value1, sizeof(value1));
    Tree::Iterator* iter = tree.find(&value1, sizeof(value1));
    tree.insert(iter, 0, &value2, sizeof(value2));

    Tree::Iterator* iter2 = tree.find(&value2, sizeof(value2));
    assertEqual(iter2 != nullptr, "Find value2 before removal");

    assertEqual(!tree.remove(iter, 1), "Fail to remove non-leaf node with leaf_only flag");
    assertEqual(tree.find(&value1, sizeof(value1)) != nullptr, "Find value1 after failed removal");

    assertEqual(tree.remove(iter, 0), "Remove node with subtree");
    assertEqual(tree.find(&value1, sizeof(value1)) == nullptr, "Find value1 after removal");
    assertEqual(tree.find(&value2, sizeof(value2)) == nullptr, "Find value2 after removal");

    delete iter;
    delete iter2;
    delete rootIter;
}

void testClearTree() {
    Mem mem(1000);
    Tree tree(mem);

    int value1 = 10;
    int value2 = 20;

    Tree::Iterator* rootIter = tree.newIterator();
    tree.insert(rootIter, 0, &value1, sizeof(value1));
    Tree::Iterator* iter = tree.find(&value1, sizeof(value1));
    tree.insert(iter, 0, &value2, sizeof(value2));

    tree.clear();
    assertEqual(tree.find(&value1, sizeof(value1)) == nullptr, "Find value1 after clear");
    assertEqual(tree.find(&value2, sizeof(value2)) == nullptr, "Find value2 after clear");

    delete iter;
    delete rootIter;
}

void testEmptyTree() {
    Mem mem(1000);
    Tree tree(mem);

    assertEqual(tree.empty(), "Tree is initially empty");

    int value = 10;
    Tree::Iterator* rootIter = tree.newIterator();
    tree.insert(rootIter, 0, &value, sizeof(value));

    assertEqual(!tree.empty(), "Tree is not empty after insertion");

    tree.clear();
    assertEqual(tree.empty(), "Tree is empty after clear");

    delete rootIter;
}

int main() {
    cout << "Running Tree tests..." << endl;
    testInsertAndFind();
    testRemoveLeafNode();
    testRemoveNonLeafNode();
    testClearTree();
    testEmptyTree();
    cout << "All tests completed." << endl;
    return 0;
}
