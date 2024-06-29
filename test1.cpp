#include <iostream>
#include "derevo.h"
#include "Mem.h"   
#include "chrono" 
using namespace std;

void test1() {
    Mem mem(999);
    Tree tree(mem);
    int value1 = 10;
    int value2 = 20;

    // Initial state
    if (tree.empty()) {
        std::cout << "PASS: Tree is initially empty" << std::endl;
    } else {
        std::cout << "FAIL: Tree is initially empty" << std::endl;
    }

    // Insert value1 at root
    AbstractTree::Iterator* rootIter = tree.newIterator();
    std::cout << "Attempting to insert value1 at root" << std::endl;
    if (tree.insert(rootIter, 0, &value1, sizeof(value1)) == 0) {
        AbstractTree::Iterator* foundIter = tree.find(&value1, sizeof(value1));
        if (foundIter) {
            std::cout << "PASS: Insert and Find value1" << std::endl;
            delete foundIter;
        } else {
            std::cout << "FAIL: Insert and Find value1" << std::endl;
        }
    } else {
        std::cout << "FAIL: Insert and Find value1" << std::endl;
    }
    delete rootIter;

    // Insert value2 as a child of value1
    rootIter = tree.find(&value1, sizeof(value1));
    std::cout << "Attempting to insert value2 as a child of value1" << std::endl;
    if (rootIter && tree.insert(rootIter, 0, &value2, sizeof(value2)) == 0) {
        AbstractTree::Iterator* foundIter = tree.find(&value2, sizeof(value2));
        if (foundIter) {
            std::cout << "PASS: Insert and Find value2" << std::endl;
            delete foundIter;
        } else {
            std::cout << "FAIL: Insert and Find value2" << std::endl;
        }
    } else {
        std::cout << "FAIL: Insert and Find value2" << std::endl;
    }
    delete rootIter;

    // Check if tree is not empty after insertion
    if (!tree.empty()) {
        std::cout << "PASS: Tree is not empty after insertion" << std::endl;
    } else {
        std::cout << "FAIL: Tree is not empty after insertion" << std::endl;
    }

    // Clear the tree
    tree.clear();
    if (tree.empty()) {
        std::cout << "PASS: Tree is empty after clear" << std::endl;
    } else {
        std::cout << "FAIL: Tree is empty after clear" << std::endl;
    }
}

int main() {
    test1();
    return 0;
}
