#include "derevo.h"
#include "queue"
using namespace std;

int Tree:: insert(AbstractTree::Iterator* iter, int child_index, void* elem, size_t size) {
TreeIterator* treeIter = dynamic_cast<TreeIterator*>(iter);
    if (!treeIter && iter == nullptr && Root == nullptr) {
        Node* newNode = new Node(elem, _memory);
        newNode->index = 0;
        Root = newNode;
        return 0;
    } else if (!treeIter) {
        std::cout << "Insert failed: Invalid iterator type" << std::endl;
        return 1;
    }

    Node* parentNode = treeIter->curNode;
    if (!parentNode && Root != nullptr) {
        std::cout << "Insert failed: Invalid parent node" << std::endl;
        return 1;
    }

    Node* newNode = new Node(elem, _memory);
    newNode->index = child_index;

    if (parentNode) {
        parentNode->children->push_front(newNode, sizeof(Node*));
    } else {
        Root = newNode;
    }

    return 0;
}

void Tree::deleteSubtree(Node* node){
    if(!node) return;
    size_t size;
    List::Iterator* childIterator = node ->children->newIterator();
    while (childIterator&&childIterator->hasNext()) {
        Node* childNode = static_cast <Node*>(childIterator->getElement(size));
        deleteSubtree(childNode);
        childIterator->goToNext();
    }
    delete node->children;
    _memory.freeMem(node->leaf);
    _memory.freeMem(node);
}

bool Tree::remove(Iterator *iter, int leaf_only){
    TreeIterator* treeIter = dynamic_cast<TreeIterator*>(iter);
        if (!treeIter) return false;

        Node* targetNode = treeIter->curNode;
        if (!targetNode) return false;

        if (leaf_only && targetNode->children->size() > 0) return false;

        deleteSubtree(targetNode);

        if (targetNode == Root) {
            Root = nullptr;
        }
    return true;
}

int Tree::size(){
    return size(); 
}

size_t Tree::max_bytes(){
    return size();
}

AbstractTree::Iterator* Tree::find(void *elem, size_t size){
    return findHelper(Root, elem, size);
}

Tree:: Iterator* Tree::newIterator(){
    if(Root){
        return new TreeIterator(this, Root, 0);
    }
    return nullptr;
}

void Tree::remove(Container::Iterator *iter){
    TreeIterator* treeIter = dynamic_cast<TreeIterator*>(iter);
        if (!treeIter) return;
        Node* targetNode = treeIter->curNode;
        if (!targetNode) return;
        if (0 && targetNode->children->size() > 0) return;
        deleteSubtree(targetNode);
        if (targetNode == Root) {
            Root = nullptr;
        }
    return;
}

void Tree::clear(){
    if (Root) {
        deleteSubtree(Root);
        Root = nullptr;
    }
}

bool Tree::empty(){
    return Root == nullptr;
}

bool Tree::TreeIterator::goToParent(){
    // Node* parent = findParent(curNode);
    // if (parent){
    //     curNode = parent;
    //     return true;
    // }
    return false;
}

bool Tree::TreeIterator::goToChild(int child_index){
    size_t size = 0;
    if (!curNode || !curNode->children) return false;
        List::Iterator* childIter = curNode->children->newIterator();
        for (int i = 0; i < child_index; i++) {
            if (!childIter->hasNext()) {
                tree->_memory.freeMem(childIter);
                return false;
            }
            childIter->goToNext();
        }
    curNode = static_cast<Node*>(childIter->getElement(size));
    tree->_memory.freeMem(childIter);
    return true;
}

void* Tree::TreeIterator::getElement(size_t &size){
    if(curNode){
        size = sizeof(curNode->leaf);
        return curNode ->leaf;
    }
    size = 0;
    return nullptr;
}

bool Tree::TreeIterator::hasNext(){
    return listIterator && listIterator->hasNext();
}

void Tree::TreeIterator::goToNext(){
    if(listIterator){
        size_t size = 0;
        listIterator->goToNext();
        curNode = static_cast<Node*>(listIterator->getElement(size));
    }
}

bool Tree::TreeIterator::equals(Container::Iterator *right){
    TreeIterator* rightIterator = dynamic_cast<TreeIterator*>(right);
    return rightIterator&&curNode==rightIterator->curNode&&
            listIterator==rightIterator->listIterator&&listPosition==rightIterator->listPosition;
}
List::Iterator* Tree::newListIterator(Node* node, size_t& listPosition, bool toBegin){
    return node->children->newIterator();
}

