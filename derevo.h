#pragma once
#include "TreeAbstract.h"
#include "List_1.h"

class Tree: public AbstractTree{

    public:
        Tree(MemoryManager &mem);
        ~Tree();

        class Node {
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
            List::Iterator* listIterator;
            TreeIterator(Tree* tree, Node* node, size_t listPosition)
                : tree(tree), curNode(node), listPosition(listPosition){};
            ~TreeIterator(){
                if(listIterator)
                    tree->_memory.freeMem(listIterator);
                if(parentArray)
                    tree->_memory.freeMem(parentArray);
                }
            bool goToParent();
            bool goToChild(int child_index);
            void* getElement(size_t &size) override;
            bool hasNext() override;
            void goToNext() override;
            bool equals(Container:: Iterator *right) override;

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

        int insert(Iterator *iter, int child_index, void *elem, size_t size) override;
        bool remove(Iterator *iter, int leaf_only) override;    //удаляет лист

        //Container
        int size() override;
        size_t max_bytes() override;
        Iterator* find(void *elem, size_t size) override;
        Iterator* newIterator() override;
        void deleteSubtree(Node* node);
        void remove(Container::Iterator *iter) override;   //удаления вершины рекурсивно
        void clear() override;
        bool empty() override;

    private:
    Node* Root;
    List::Iterator* newListIterator(Node* node, size_t& listPosition, bool toBegin);

};