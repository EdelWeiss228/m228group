#include "derevo.h"
#include "queue"
using namespace std;

Tree::Node* Tree::Iterator::returnNode(){
    size_t objectSize=0;
    if (currentIterator == nullptr)
        return nullptr;
    void* tmpElem = this->currentIterator->getElement(objectSize);
    return static_cast<Node*>(tmpElem);
}

int Tree:: insert(AbstractTree::Iterator* iter, int child_index, void* elem, size_t size) {
    Tree::Iterator* tmp = static_cast <Tree::Iterator*>(iter);
    
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

bool Tree::remove(AbstractTree::Iterator *iter, int leaf_only){

}

int Tree::size(){
    return NumberOfElems; 
}

size_t Tree::max_bytes(){
    return _memory.maxBytes();
}

AbstractTree::Iterator* Tree::find(void *elem, size_t size){
    return findHelper(Root, elem, size);
}

Tree:: Iterator* Tree::newIterator(){
    Iterator* newIterator = new Iterator();
    if (newIterator == nullptr)
        return nullptr;
    if(!(this->empty())){
        newIterator->currentIterator = (List::Iterator*) Root->newIterator();
        return newIterator;
    }
    else{
        delete newIterator;
        return nullptr;
    }
}

void Tree::remove(Container::Iterator *iter){
    Iterator* treeIter = dynamic_cast<Iterator*>(iter);
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
    if(!this->empty()){
        Iterator* tmp = this->newIterator();
        remove(tmp);
    }
}

bool Tree::empty(){
    return Root == nullptr;
}

bool Tree::Iterator::goToParent(){
    if(parentIterator==nullptr && currentIterator == nullptr && this->returnNode() == nullptr)
        return false;
    delete this->currentIterator;
    Iterator* tmpParent = this->parentIterator;
    this->parentIterator=tmpParent->parentIterator;
    this->currentIterator=tmpParent->currentIterator;
    tmpParent->parentIterator = nullptr;
    tmpParent->currentIterator = nullptr;
    delete tmpParent;
    return true;
}

bool Tree::Iterator::goToChild(int child_index){
    Node* curNode = this->returnNode();
    if (curNode == nullptr && curNode->children->empty())
        return false;
    this->parentIterator = new Iterator(*this);
    this->currentIterator = dynamic_cast <List::Iterator*>(curNode->children->newIterator());
    return true;
}

void* Tree::Iterator::getElement(size_t &size){
    if(currentIterator == nullptr){
        size = 0;
        return nullptr;
    }
    Node* curNode = this->returnNode();
    size = curNode->size;
    return curNode ->leaf;
}

bool Tree::Iterator::hasNext(){
    Node* curNode = this->returnNode();
    if ((curNode == nullptr)||(curNode->children->empty())||(currentIterator->hasNext()))
        return false;
    else{
        Iterator* tmp = this->parentIterator;
        while (!(tmp == nullptr)){
            if (tmp->currentIterator->hasNext()){
                return true;
            }
            tmp = tmp->parentIterator;
        }
    }
    return false;
}

void Tree::Iterator::goToNext(){
    if((this->returnNode() == nullptr)||(this->goToChild(0)))
        return;
    if (this->currentIterator->hasNext()) {
        this->currentIterator->goToNext();
    }
    else {
        while (this->goToParent()){
            if(this->currentIterator->hasNext()){
                this->currentIterator->goToNext();
                break;
            }
        }
        if(!(this->returnNode()->parent)){
            delete this->currentIterator;
            this->currentIterator=nullptr;
        }
    }  
}

bool Tree::Iterator::equals(Container::Iterator *right){
   Iterator* rightIterator = dynamic_cast<Iterator*>(right);
   return this->returnNode()==rightIterator->returnNode();
}

