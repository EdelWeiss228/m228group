#include <gtest/gtest.h>
#include "derevo.h"  // Include the Tree class definition
#include "Mem.h"     // Include the MemoryManager class definition

class TreeTest : public ::testing::Test {
protected:
    MemoryManager mem;
    Tree* tree;

    virtual void SetUp() {
        tree = new Tree(mem);
    }

    virtual void TearDown() {
        delete tree;
    }
};

TEST_F(TreeTest, InsertAndFind) {
    int value1 = 10;
    int value2 = 20;
    size_t size;

    Tree::Iterator* rootIter = tree->newIterator();
    tree->insert(rootIter, 0, &value1, sizeof(value1));
    Tree::Iterator* iter = tree->find(&value1, sizeof(value1));
    ASSERT_NE(iter, nullptr);

    tree->insert(iter, 0, &value2, sizeof(value2));
    Tree::Iterator* iter2 = tree->find(&value2, sizeof(value2));
    ASSERT_NE(iter2, nullptr);

    delete iter;
    delete iter2;
    delete rootIter;
}

TEST_F(TreeTest, RemoveLeafNode) {
    int value1 = 10;
    int value2 = 20;

    Tree::Iterator* rootIter = tree->newIterator();
    tree->insert(rootIter, 0, &value1, sizeof(value1));
    Tree::Iterator* iter = tree->find(&value1, sizeof(value1));
    tree->insert(iter, 0, &value2, sizeof(value2));

    Tree::Iterator* iter2 = tree->find(&value2, sizeof(value2));
    ASSERT_NE(iter2, nullptr);

    ASSERT_TRUE(tree->remove(iter2, 1));  // Remove leaf node
    ASSERT_EQ(tree->find(&value2, sizeof(value2)), nullptr);

    delete iter;
    delete iter2;
    delete rootIter;
}

TEST_F(TreeTest, RemoveNonLeafNode) {
    int value1 = 10;
    int value2 = 20;

    Tree::Iterator* rootIter = tree->newIterator();
    tree->insert(rootIter, 0, &value1, sizeof(value1));
    Tree::Iterator* iter = tree->find(&value1, sizeof(value1));
    tree->insert(iter, 0, &value2, sizeof(value2));

    Tree::Iterator* iter2 = tree->find(&value2, sizeof(value2));
    ASSERT_NE(iter2, nullptr);

    ASSERT_FALSE(tree->remove(iter, 1));  // Attempt to remove non-leaf node with leaf_only flag
    ASSERT_NE(tree->find(&value1, sizeof(value1)), nullptr);

    ASSERT_TRUE(tree->remove(iter, 0));  // Remove node with subtree
    ASSERT_EQ(tree->find(&value1, sizeof(value1)), nullptr);
    ASSERT_EQ(tree->find(&value2, sizeof(value2)), nullptr);

    delete iter;
    delete iter2;
    delete rootIter;
}

TEST_F(TreeTest, ClearTree) {
    int value1 = 10;
    int value2 = 20;

    Tree::Iterator* rootIter = tree->newIterator();
    tree->insert(rootIter, 0, &value1, sizeof(value1));
    Tree::Iterator* iter = tree->find(&value1, sizeof(value1));
    tree->insert(iter, 0, &value2, sizeof(value2));

    tree->clear();
    ASSERT_EQ(tree->find(&value1, sizeof(value1)), nullptr);
    ASSERT_EQ(tree->find(&value2, sizeof(value2)), nullptr);

    delete iter;
    delete rootIter;
}

TEST_F(TreeTest, EmptyTree) {
    ASSERT_TRUE(tree->empty());

    int value = 10;
    Tree::Iterator* rootIter = tree->newIterator();
    tree->insert(rootIter, 0, &value, sizeof(value));

    ASSERT_FALSE(tree->empty());

    tree->clear();
    ASSERT_TRUE(tree->empty());

    delete rootIter;
}

TEST_F(TreeTest, DestructorTest) {
    int value1 = 10;
    int value2 = 20;

    tree->insert(tree->newIterator(), 0, &value1, sizeof(value1));
    Tree::Iterator* iter = tree->find(&value1, sizeof(value1));
    tree->insert(iter, 0, &value2, sizeof(value2));

    // Destructor should clear the tree without issues
    // Since it's a destructor test, we don't have a direct assertion
    // Just ensure no memory leaks or crashes
}
