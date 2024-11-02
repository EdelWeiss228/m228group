#pragma once
#include "TreeAbstract.h"
#include "LinkedList1.hpp"

class Tree: public AbstractTree{

    public:
        Tree(MemoryManager &mem) : AbstractTree(mem), Root{mem}, NumberOfElems{0} {}
        ~Tree(){Root.removeChildren(this->_memory);}

        class Node {
            public:
            Node(MemoryManager &mem): children(mem){}
            ~Node(){}
            List children;
            Node* parent = nullptr;
            void* leaf=nullptr;
            size_t leafSize=0;
            Node* find (size_t size, void* elem);
            size_t removeChildren (MemoryManager &mem);
            bool isLeaf();
            Node* find (void* data, size_t size);
            Node* getFirst();
            size_t rightElemIndex (Node* node);
        };

        class Iterator: public AbstractTree::Iterator{
            private:
            const Node *Root;
            Node *curNode;

            public:
            Iterator(const Node* root, Node* node):Root{root}, curNode{node}{}
            ~Iterator() = default;
            Node* returnNode(){return curNode;}
            bool goToParent() override;
            bool goToChild(int child_index) override;
            void* getElement(size_t &size) override;
            bool hasNext() override;
            void goToNext() override;
            bool equals(Container:: Iterator *right) override;

            const bool operator==(Container:: Iterator *right){
                return equals(right);
            }
            friend class Tree;
        };

        int insert(AbstractTree::Iterator *iter, int child_index, void *elem, size_t size) override;
        bool remove(AbstractTree::Iterator *iter, int leaf_only) override;    //удаляет лист

        //Container
        int size() override;
        size_t max_bytes() override;
        Container:: Iterator* find(void *elem, size_t size) override;
        Iterator* newIterator() override;
        void remove(Container::Iterator *iter) override;   //удаления вершины рекурсивно
        void clear() override;
        bool empty() override;
        friend class Iterator;

    private:
    Iterator* createIterator (Node *node);
    Node Root;
    size_t NumberOfElems = 0;
};