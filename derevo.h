#pragma once
#include "TreeAbstract.h"
#include "List_1.h"

class Tree: public AbstractTree{

    public:
        Tree(MemoryManager &mem) : AbstractTree(mem), Root(nullptr) {}
        ~Tree(){ clear(); }

        class Node {
            private:
            Tree* tree;

            public:
            void* leaf;
            List* children;
            int index;
            Node(void* value, MemoryManager& mem) : leaf(value), children(new List(mem)), index(0) {}
            ~Node() {
                tree->_memory.freeMem(children);
            }
        };

        class TreeIterator: public AbstractTree::Iterator{
            private:
            Tree* tree;
            size_t listPosition;

            public:
            Node* curNode;
            List::Iterator* listIterator;
            TreeIterator(Tree* tree, Node* node, size_t listPosition)
                : tree(tree), curNode(node), listPosition(listPosition){
                    if(curNode){
                        listIterator=curNode->children->newIterator();
                    }  else {
                            listIterator = nullptr;
                    }
                };
            ~TreeIterator(){
                if(listIterator)
                    tree->_memory.freeMem(listIterator);
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
        Iterator* findHelper(Node* node, void* elem, size_t size) {
        size_t size_o = 0;
        if (!node) return nullptr;
        if (memcmp(node->leaf, elem, size) == 0) return new TreeIterator(this, node, 0);
        List::Iterator* iter = node->children->newIterator();
        Iterator* result = nullptr;
        while (iter->hasNext()) {
            iter->goToNext();
            result = findHelper(static_cast<Node*>(iter->getElement(size_o)), elem, size);
            if (result) break;
        }
         _memory.freeMem(iter);
        return result;
    }
};