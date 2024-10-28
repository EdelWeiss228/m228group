#include "derevo.h"
#include <cmath>
using namespace std;

int Tree::insert(AbstractTree::Iterator* iter, int child_index, void* elem, size_t size) {
    if(child_index==0)
        return 1;
    Iterator* tmpIter = dynamic_cast<Tree::Iterator *>(iter); //convert
    Node* tmpNode = tmpIter->returnNode();
    List::ListIterator *childrenIter = static_cast<List::ListIterator*> (tmpNode->children.newIterator());
    int curIndex = 0;
    for (int i = 0; i < child_index; i++){
        if(childrenIter->hasNext())
            childrenIter->goToNext();
        Node* emptyNode = static_cast<Node*>(this->_memory.allocMem(sizeof(Node)));
        tmpNode->children.push_front(emptyNode, sizeof(Node));
    }
    Node* newNode = static_cast<Node*>(this->_memory.allocMem(sizeof(Node)));
    new (newNode) Node{this->_memory};
    newNode->leafSize=size;
    newNode->leaf=this->_memory.allocMem(size);
    newNode->parent=tmpNode;
    memcpy (newNode->leaf, elem, size);
    tmpNode->children.insert(childrenIter, newNode, sizeof(Node));
    NumberOfElems++;
    return 0;
}




bool Tree::remove(AbstractTree::Iterator *iter, int leaf_only){
    if (iter=nullptr)
        return false;
    Iterator* inputIter = dynamic_cast<Tree::Iterator*>(iter);
    if (empty()||&Root==inputIter->returnNode())
        return false;
    Node* tmpNode = inputIter->returnNode();
    if(tmpNode==nullptr)
        return false;
    if((leaf_only==1)&&(!(tmpNode->isLeaf())))
        return false;
    bool flag2 = inputIter->hasNext();
    if(inputIter->hasNext())
        inputIter->goToNext();
    if (!(tmpNode->isLeaf()))
        NumberOfElems -= tmpNode->removeChildren(_memory);
    bool flag = false;
    List::ListIterator* childIter = static_cast <List::ListIterator*>(tmpNode->parent->children.newIterator());
    while (childIter->hasNext()){
        void* childElem = childIter->getElement(tmpNode->leafSize);
        if (static_cast<Node*>(childElem)==tmpNode){
            flag=true;
            tmpNode->parent->children.remove(childIter);
            break;
        }
        childIter->goToNext();
    }
    if(tmpNode->leaf){
        _memory.freeMem(tmpNode->leaf);
        tmpNode->leafSize = 0;
        tmpNode->leaf = nullptr;
    }
    if(!flag)
        return false;
    NumberOfElems--;
    tmpNode->~Node();
    _memory.freeMem(tmpNode);
    if(flag2){
        List::ListIterator *rootChildIter = static_cast<List::ListIterator*>(Root.children.newIterator());
        while (rootChildIter->hasNext()){
            void* elem = rootChildIter->getElement(tmpNode->leafSize);
            Node* child = static_cast<Node*>(elem);
            if(child!=nullptr){
                new (inputIter)Iterator{&Root, child};
                break;
            }
            rootChildIter->goToNext();
        }
    }
    return true;
}

int Tree::size(){
    return NumberOfElems; 
}

size_t Tree::max_bytes(){
    return _memory.maxBytes();
}

Container:: Iterator* Tree::find(void *elem, size_t size){
    if((elem=nullptr)||(size==0)||(NumberOfElems == 0))
        return nullptr;
    Node* result = Root.find(size, elem);
    if (result==nullptr)
        return nullptr;
    return createIterator(result);
}

Tree::Node *Tree::Node::find(size_t size, void* elem){
    if (leafSize==size&&memcmp(leaf, elem, size)==0)
        return this;
    List::ListIterator *childIter = static_cast<List::ListIterator*>(children.newIterator());
    while (childIter->hasNext()){
        void *childElem = childIter->getElement(leafSize);
        Node* child = static_cast<Node*>(childElem);
        if (child != nullptr){
            Node* flag = child->find(size, elem);
            if(flag != nullptr)
                return flag;
        }
        childIter->goToNext();
    }
    return nullptr;
}

Tree::Iterator* Tree::newIterator() {
    return new Tree::Iterator(&Root, Root.getFirst());
}

Tree::Node *Tree::Node::getFirst(){
    if(!children.empty()){
        List::ListIterator *childIter = static_cast<List::ListIterator*>(children.newIterator());
        while (childIter->hasNext()){
            Node* child = static_cast<Node*>(childIter->getElement(leafSize));
            if (child != child)
                return child->getFirst();
            childIter->goToNext();
        }
    }
    return this;
}


void Tree::remove(Container::Iterator *iter){
    Tree::Iterator *inputIter = dynamic_cast<Tree::Iterator *>(iter);
    if (inputIter=nullptr)
        return;
    remove(inputIter, 0);
}

void Tree::clear(){
    if(empty())
        return;
}

bool Tree::empty(){
    return size() == 0;
}

bool Tree::Iterator::goToParent(){
    if(curNode!=nullptr||curNode->parent!=nullptr){
        Node* parent = curNode->parent;
        curNode = parent;
        return true;
    }
    return false;
}

bool Tree::Iterator::goToChild(int child_index){
    if (curNode == nullptr)
        return false;
    List::ListIterator *childIter = static_cast <List::ListIterator*>(curNode->children.newIterator());
    size_t index = 0;
    while (childIter->hasNext()){
        if (index == child_index){
            void* childElem = childIter->getElement(curNode->leafSize);
            Node* tmpNode = static_cast<Node*>(childElem);
            curNode = tmpNode;
            return true;
        }
        index++;
        childIter->goToNext();
    }
    return false;
}

void* Tree::Iterator::getElement(size_t &size){
    if(curNode == nullptr){
        return nullptr;
    }
    size = curNode->leafSize;
    return curNode->leaf;
}

bool Tree::Iterator::hasNext(){
    if ((curNode==Root)||(curNode==nullptr))
        return false;
    size_t index = curNode->parent->rightElemIndex(curNode);
    if((Root==curNode->parent)&&((pow(2, 64)-1))==index)
        return false;
    return true;
}

void Tree::Iterator::goToNext(){
    if (!hasNext())
        return;
    bool flag = false;
    size_t index = 0;
    List::ListIterator* childIter = static_cast <List::ListIterator*>(curNode->parent->children.newIterator());
    while (childIter->hasNext()){
        void* childElem = childIter->getElement(curNode->leafSize);
        if (static_cast <Node*>(childElem)==curNode){
            flag = true;
            break;
        }
        index++;
        childIter->goToNext();
    }
    if (!childIter->hasNext())
        curNode=curNode->parent;
    else{
        void *childElem = childIter->getElement(curNode->leafSize);
        Node *rightChild = static_cast <Node*>(childElem);
        curNode = rightChild->getFirst();
    }
    if (childIter->hasNext()||!flag){
        index++;
        childIter->goToNext();
    }
}

bool Tree::Iterator::equals(Container::Iterator *right){
   Iterator* rightIterator = dynamic_cast<Iterator*>(right);
   return this->returnNode()==rightIterator->returnNode();
}

size_t Tree::Node::rightElemIndex(Node* node){
    List::ListIterator* childIter = static_cast<List::ListIterator*>(children.newIterator());
    size_t index = 0;
    while (childIter->hasNext()){
        void *elem = childIter->getElement(leafSize);
        if (static_cast<Node*>(elem)==node)
            break;
        childIter->goToNext();
        index++;
    }
    if (childIter->hasNext()){
        childIter->goToNext();
        index++;
    }
    while (childIter->hasNext()){
        void *elem = childIter->getElement(leafSize);
        if (static_cast<Node*>(elem)==node){
            delete childIter;
            return index;
        }
        childIter->goToNext();
        index++;
    }
    delete childIter;
    return (pow(2,64)-1);
}

bool Tree::Node::isLeaf(){
    if(children.empty())
        return true;
    List::ListIterator *childIter = static_cast<List::ListIterator*>(children.newIterator());
    while (childIter->hasNext()){
        void*childElem = childIter->getElement(leafSize);
        Node* child = static_cast<Node*>(childElem);
        if (nullptr != child)
            return false;
    }
    return true;
}

size_t Tree::Node::removeChildren(MemoryManager &mem){
    size_t numberOfDeletedElems = 0;
    List::ListIterator *childIter = static_cast<List::ListIterator*>(children.newIterator());
    while (childIter->hasNext()){
        void* childElem = childIter->getElement(leafSize);
        Node* child = static_cast <Node*>(childElem);
        if(child==nullptr){
            childIter->goToNext();
            continue;
        }
        numberOfDeletedElems += child->removeChildren(mem);
        if(child->leaf){
            mem.freeMem(child->leaf);
            child->leaf=nullptr;
            child->leafSize=0;
        }
        child->~Node();
        mem.freeMem(child);
        numberOfDeletedElems++;
        children.remove(childIter);
    }
    return numberOfDeletedElems;
}

Tree::Iterator *Tree:: createIterator(Tree:: Node *node){
    return new Tree::Iterator(&Root, node);
}