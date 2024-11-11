#pragma once
#include "TreeAbstract.h"
#include "List_1.h"
using namespace std;

class Tree : public AbstractTree {
public:
    Tree(MemoryManager& mem) : AbstractTree(mem), _Root{ mem }, _NumberOfElems{ 0 } {}
    ~Tree() {
        _Root.removeChildren(this->_memory);
    };

    class Node {
    public:
        Node(MemoryManager& mem) : children(mem) {};
        ~Node() {};
        List children;
        Node* parent = nullptr;
        void* leaf = nullptr;
        size_t leafSize = 0;
        Node* find(size_t size, void* elem);
        size_t removeChildren(MemoryManager& mem);
        bool isLeaf();
        Node* getFirst();
        size_t rightElemIndex(Node* node);
    };

    class Iterator : public AbstractTree::Iterator {
    private:
        const Node* _Root;
        Node* _curNode;

    public:
        Iterator(const Node* root, Node* node) : _Root{ root }, _curNode{ node } {};
        ~Iterator() = default;
        Node* returnNode() { return this->_curNode; };
        bool goToParent() override;
        bool goToChild(int child_index) override;
        void* getElement(size_t& size) override;
        bool hasNext() override;
        void goToNext() override;
        bool equals(Container::Iterator* right) override;
    };

    int insert(AbstractTree::Iterator* iter, int child_index, void* elem, size_t size) override;
    bool remove(AbstractTree::Iterator* iter, int leaf_only) override;

    int size() override;
    size_t max_bytes() override;
    Container::Iterator* find(void* elem, size_t size) override;
    Iterator* newIterator() override;
    void remove(Container::Iterator* iter) override;
    void clear() override;
    bool empty() override;

private:
    Iterator* createIterator(Node* node);
    Node _Root;
    size_t _NumberOfElems = 0;
};

