#include "derevo.h"
using namespace std;

Tree::Node* Tree::Iterator::returnNode(){
    size_t objectSize=0;
    if (currentIterator == nullptr)
        return nullptr;
    void* tmpElem = this->currentIterator->getElement(objectSize);
    return static_cast<Node*>(tmpElem);
}

int Tree:: insert(AbstractTree::Iterator* iter, int child_index, void* elem, size_t size) {
    Iterator* tmp = static_cast <Iterator*>(iter);
    bool emptinessFlag = true;
    if (!tmp) {
        if (empty())
            return 1;
        emptinessFlag = false;
    }
    Node* tmpNode = nullptr;
    List* tmpList = nullptr;
    if (!emptinessFlag) {
        tmpNode = tmp->returnNode();
        if(!tmpNode)
            return 1;
        tmpList = tmpNode->children;
    }
    else tmpList = this->Root;
    void* tmpChildren = _memory.allocMem(sizeof(List));
    void* tmpObject = _memory.allocMem(size);
    if (!tmpChildren)
        _memory.freeMem(tmpChildren);
    else if (!tmpObject)
        _memory.freeMem(tmpObject);
    else if (tmpObject && tmpChildren){
        new (tmpChildren) List(this->_memory);
        Node* newObject = new Node {tmpObject, static_cast<List*>(tmpChildren), size, tmpNode};
        memcpy(tmpObject, elem, size);
        if (!newObject){
            _memory.freeMem(tmpChildren);
            _memory.freeMem(tmpObject);
        }
        else{
            tmpList->push_front(newObject, sizeof(*newObject));
            delete newObject;
            NumberOfElems++;
            return 0;
        }
    }
    return 1;
}

bool Tree::remove(AbstractTree::Iterator *iter, int leaf_only){
    Iterator* treeIter = dynamic_cast<Iterator*>(iter);
    if(!treeIter)
        return false;
    Node* tmpNode = treeIter->returnNode();
    if(!tmpNode)
        return false;
    if(!leaf_only){
        remove (iter);
        return true;
    }
    else{
        if(!tmpNode->children->empty())
            return false;
        Node* tmpParent = tmpNode->parent;
        this->_memory.freeMem(tmpNode->leaf);
        tmpNode->children->~List();
        this->_memory.freeMem(tmpNode->children);
        if (!tmpParent) {
            delete treeIter;
            Root->clear();
        }
        else {
            if(tmpParent->children->size() <= 1){
                treeIter->goToParent();
                tmpParent->children->clear();
            }
            else
                tmpParent->children->remove(treeIter->currentIterator);
        }
        this->NumberOfElems-1;
        return true;
    }
}

int Tree::size(){
    return NumberOfElems; 
}

size_t Tree::max_bytes(){
    return _memory.maxBytes();
}

Tree::Iterator* Tree::find(void *elem, size_t size){
    
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
    if(treeIter){
        Node* tmpNode = treeIter->returnNode();
        if (!tmpNode)
            return;
        if (treeIter->goToChild(0)){
            while(!tmpNode->children->empty()){
                if(treeIter->goToChild(0))
                    remove(treeIter);
                remove(treeIter, 1);
            }
        }
        remove(treeIter, 1);
    }
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

