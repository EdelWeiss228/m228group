#pragma once
#include "TreeAbstract.h"

typedef struct Node{
    void* leaf;
    list<void*> children;
    int index;
};

class tree: public AbstractTree{

    private:
    List::Iterator* newListIterator(size_t& listPosition, bool toBegin);

    public:
        tree(MemoryManager &mem): AbstractTree(mem) {}
        ~tree() {}

        class TreeIterator: public AbstractTree::Iterator{
            private:
            tree* Tree;
            Iterator* listIterator;
            size_t listPosition;

            public:

            TreeIterator(tree* Tree, Iterator* it, size_t listPosition): listIterator(it), Tree(Tree), listPosition(listPosition){};
            ~TreeIterator(){if(listIterator) Tree->_memory.freeMem(listIterator);}
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
};