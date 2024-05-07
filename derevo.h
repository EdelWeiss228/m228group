#pragma once
#include "TreeAbstract.h"
#include "List.h"

class Tree: public AbstractTree{

    public:
        Tree(MemoryManager &mem): AbstractTree(mem) {Root=nullptr;}
        ~Tree() {}

        class Node{
            public:
            void* leaf;
            List* children;
            int index;
            Node(void* value, MemoryManager& mem) {
                children = new List(mem);
                leaf = value;
            }
        };

        class TreeIterator: public AbstractTree::Iterator{
            private:
            Tree* tree;
            Iterator* listIterator;
            size_t listPosition;

            public:

            TreeIterator(Tree* tree, Iterator* iterator, size_t listPosition): listIterator(iterator), tree(tree), listPosition(listPosition){};
            ~TreeIterator(){if(listIterator) tree->_memory.freeMem(listIterator);}
            bool goToParent();
            bool goToChild(int child_index);
            void* getElement(size_t &size);
            bool hasNext();
            void goToNext();
            bool equals(Container:: Iterator *right);

            const bool operator==(Container:: Iterator *right){
                return equals(right);
            }
        };

        int insert(Iterator *iter, int child_index, void *elem, size_t size);
        bool remove(Iterator *iter, int leaf_only);    //удаляет лист

        //Container
        int size();
        size_t max_bytes();
        Iterator* find(void *elem, size_t size);
        Iterator* newIterator(); 
        void remove(Container::Iterator *iter);    //удаления вершины рекурсивно
        void clear();
        bool empty();

    private:
    Node* Root;
    List::Iterator* newListIterator(size_t& listPosition, bool toBegin);

};