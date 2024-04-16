#pragma once
#include "TreeAbstract.h"

typedef struct Node{
    
};

class tree: public AbstractTree{
private:
tree* Tree;
    public:
        tree(MemoryManager &mem): AbstractTree(mem) {}
        ~tree() {}

        class TreeIterator: public AbstractTree::Iterator{
            private:
            Iterator* listIterator;
            TTree *l;
            TTree::iterator it;

            public:

            TreeIterator(TList *l) : l(l), it(l->begin()) {}
            bool goToParent();
            bool goToChild(int child_index);
            void* getElement(size_t &size);
            bool hasNext();
            void goToNext();
            bool equals(Iterator *right);
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