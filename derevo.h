#pragma once
#include "TreeAbstract.h"
#include "List_1.h"

class Tree: public AbstractTree{

    public:
        Tree(MemoryManager &mem): AbstractTree(mem) {Root=nullptr;}
        ~Tree() {}

        class Node{
            private:
            Tree* tree;

            public:
            void* leaf;
            List* children;
            int index;
            Node(void* value, MemoryManager& mem) {
                children = new List(mem);
                leaf = value;
            }
            ~Node() {
                tree->_memory.freeMem(children);
            }
        };

        class TreeIterator: public AbstractTree::Iterator{
            private:
            Tree* tree;
            
            List::Iterator* listIterator;
            size_t listPosition;

            struct ParentInfo
            {
                Node* child;
                Node* parent;
            };

            ParentInfo* parentArray;
            size_t actualParentArraySize;
            size_t fullParentArrayCap;

            void resizeParentArray();

            public:
            Node* curNode;
            TreeIterator(Tree* tree, Node* node, List::Iterator* iterator, size_t listPosition)
                : tree(tree), curNode(node), listIterator(iterator), listPosition(listPosition){};
            ~TreeIterator(){
                if(listIterator)
                    tree->_memory.freeMem(listIterator);
                if(parentArray)
                    tree->_memory.freeMem(parentArray);
                }
            bool goToParent();
            bool goToChild(int child_index);
            void* getElement(size_t &size);
            bool hasNext();
            void goToNext();
            bool equals(Container:: Iterator *right);

            const bool operator==(Container:: Iterator *right){
                return equals(right);
            }

            void addParentInfo(Node* child, Node* parent){
                if (actualParentArraySize==fullParentArrayCap)
                    resizeParentArray();
                parentArray[actualParentArraySize+1]={child, parent};
                actualParentArraySize++;
            }

            Node* findParent(Node* child){
                for (size_t i = 0; i < actualParentArraySize; i++)
                {
                    if(parentArray[i].child == child)
                        return parentArray[i].parent;
                    else
                        return nullptr;
                }
                
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