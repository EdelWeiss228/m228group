#pragma once
#include "TreeAbstract.h"
#include "List_1.h"

class Tree: public AbstractTree{

    public:
        Tree(MemoryManager &mem) : AbstractTree(mem), Root(nullptr), NumberOfElems(0) {new (Root) List(this->_memory);}
        ~Tree(){ this->clear(); this->Root->~List(); this->_memory.freeMem(Root); this->Root=nullptr; AbstractTree::~AbstractTree();}

        struct Node {
            void* leaf;
            List* children;
            int index;
            size_t size;
            Node* parent;
        };

        class Iterator: public AbstractTree::Iterator{
            private:
            Tree* tree;
            Node* returnNode();
            Iterator* parentIterator;
            List::Iterator* currentIterator;

            public:
            List::Iterator* listIterator;
            Iterator(){this->parentIterator=nullptr; this->currentIterator = nullptr;}
            ~Iterator(){while (this->goToParent()) continue; delete this->currentIterator; delete this->parentIterator;}
            bool goToParent();
            bool goToChild(int child_index);
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
        Iterator* find(void *elem, size_t size) override;
        Iterator* newIterator() override;
        void deleteSubtree(Node* node);
        void remove(Container::Iterator *iter) override;   //удаления вершины рекурсивно
        void clear() override;
        bool empty() override;
        friend class Iterator;

    private:
    List* Root;
    size_t NumberOfElems;
};