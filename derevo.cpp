#include "derevo.h"
#include <cmath>
using namespace std;

int Tree::insert(AbstractTree::Iterator* iter, int child_index, void* elem, size_t size) {
    if(child_index < 0)
        return 1;
    Iterator* tmpIter = dynamic_cast<Tree::Iterator *>(iter);
    if (!tmpIter) return 1;
    Node* tmpNode = tmpIter->returnNode();
    if (!tmpNode) return 1;

    // Fill with empty nodes if child_index is beyond current size
    while (tmpNode->children.size() < child_index) {
        Node* emptyNode = static_cast<Node*>(this->_memory.allocMem(sizeof(Node)));
        new (emptyNode) Node(this->_memory);
        emptyNode->parent = tmpNode;
        tmpNode->children.push_front(&emptyNode, sizeof(Node*)); 
    }

    List::ListIterator *childrenIter = static_cast<List::ListIterator*> (tmpNode->children.newIterator());
    for (int i = 0; i < child_index && childrenIter && childrenIter->hasNext(); i++){
        childrenIter->goToNext();
    }

    Node* newNode = static_cast<Node*>(this->_memory.allocMem(sizeof(Node)));
    new (newNode) Node{this->_memory};
    newNode->leafSize=size;
    newNode->leaf=this->_memory.allocMem(size);
    newNode->parent=tmpNode;
    memcpy (newNode->leaf, elem, size);
    
    if (childrenIter) {
        tmpNode->children.insert(childrenIter, &newNode, sizeof(Node*));
        delete childrenIter;
    } else {
        tmpNode->children.push_front(&newNode, sizeof(Node*));
    }
    
    NumberOfElems++;
    return 0;
}

bool Tree::remove(AbstractTree::Iterator *iter, int leaf_only){
    if (iter==nullptr)
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
    if(flag2)
        inputIter->goToNext();
    
    if (!(tmpNode->isLeaf()))
        NumberOfElems -= tmpNode->removeChildren(_memory);
    bool flag = false;
    List::ListIterator* childIter = static_cast <List::ListIterator*>(tmpNode->parent->children.newIterator());
    while (childIter && childIter->hasNext()){
        void* childData = childIter->getElement(tmpNode->leafSize);
        Node* currentChild = (childData) ? *(static_cast<Node**>(childData)) : nullptr;
        if (currentChild == tmpNode){
            flag=true;
            tmpNode->parent->children.remove(childIter);
            break;
        }
        childIter->goToNext();
    }
    if (childIter) delete childIter;
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
    if(!flag2){
        // If we removed the last element in the traversal, reset to root or start again
        Node* first = Root.getFirst();
        new (inputIter)Iterator{&Root, first};
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
    if((elem==nullptr)||(size==0)||(NumberOfElems == 0))
        return nullptr;
    Node* result = Root.find(size, elem);
    if (result==nullptr)
        return nullptr;
    return createIterator(result);
}

Tree::Node *Tree::Node::find(size_t size, void* elem){
    if (leaf != nullptr && leafSize == size && memcmp(leaf, elem, size) == 0)
        return this;
    List::ListIterator *childIter = static_cast<List::ListIterator*>(children.newIterator());
    while (childIter && childIter->hasNext()){
        void *childData = childIter->getElement(leafSize);
        Node* child = (childData) ? *(static_cast<Node**>(childData)) : nullptr;
        if (child != nullptr){
            Node* flag = child->find(size, elem);
            if(flag != nullptr) {
                delete childIter;
                return flag;
            }
        }
        childIter->goToNext();
    }
    if (childIter) delete childIter;
    return nullptr;
}

Tree::Iterator* Tree::newIterator() {
    return new Tree::Iterator(&Root, Root.getFirst());
}

Tree::Node *Tree::Node::getFirst(){
    if(!children.empty()){
        List::ListIterator *childIter = static_cast<List::ListIterator*>(children.newIterator());
        while (childIter && childIter->hasNext()){
            void* childData = childIter->getElement(leafSize);
            Node* child = (childData) ? *(static_cast<Node**>(childData)) : nullptr;
            if (child != nullptr) {
                Node* result = child->getFirst();
                delete childIter;
                return result;
            }
            childIter->goToNext();
        }
        if (childIter) delete childIter;
    }
    return this;
}


void Tree::remove(Container::Iterator *iter){
    Tree::Iterator *inputIter = dynamic_cast<Tree::Iterator *>(iter);
    if (inputIter==nullptr)
        return;
    remove(inputIter, 0);
}

void Tree::clear(){
    if(empty())
        return;
    Root.removeChildren(_memory);
    NumberOfElems = 0;
}

bool Tree::empty(){
    return size() == 0;
}

bool Tree::Iterator::goToParent(){
    if(curNode!=nullptr && curNode->parent!=nullptr){
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
    while (childIter && childIter->hasNext()){
        if (index == child_index){
            void* childData = childIter->getElement(curNode->leafSize);
            Node* tmpNode = (childData) ? *(static_cast<Node**>(childData)) : nullptr;
            curNode = tmpNode;
            delete childIter;
            return true;
        }
        index++;
        childIter->goToNext();
    }
    if (childIter) delete childIter;
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
    if((Root==curNode->parent)&&((size_t)-1)==index)
        return false;
    return true;
}

void Tree::Iterator::goToNext(){
    if (!hasNext())
        return;
    bool flag = false;
    size_t index = 0;
    List::ListIterator* childIter = static_cast <List::ListIterator*>(curNode->parent->children.newIterator());
    if (!childIter) return;
    while (childIter && childIter->hasNext()){
        void* childData = childIter->getElement(curNode->leafSize);
        Node* currentChild = (childData) ? *(static_cast<Node**>(childData)) : nullptr;
        if (currentChild == curNode){
            flag = true;
            break;
        }
        index++;
        childIter->goToNext();
    }
    if (!childIter || !childIter->hasNext() || !childIter->cur_node->get_next())
        curNode=curNode->parent;
    else{
        childIter->goToNext();
        void *childData = childIter->getElement(curNode->leafSize);
        Node *rightChild = (childData) ? *(static_cast<Node**>(childData)) : nullptr;
        if (rightChild)
            curNode = rightChild->getFirst();
        else
            curNode = curNode->parent;
    }
    delete childIter;
}

bool Tree::Iterator::equals(Container::Iterator *right){
   Iterator* rightIterator = dynamic_cast<Iterator*>(right);
   if (!rightIterator) return false;
   return this->curNode == rightIterator->curNode;
}

size_t Tree::Node::rightElemIndex(Node* node){
    List::ListIterator* childIter = static_cast<List::ListIterator*>(children.newIterator());
    if (!childIter) return (size_t)-1;
    size_t index = 0;
    while (childIter && childIter->hasNext()){
        void *childData = childIter->getElement(leafSize);
        Node* currentChild = (childData) ? *(static_cast<Node**>(childData)) : nullptr;
        if (currentChild == node)
            break;
        childIter->goToNext();
        index++;
    }
    if (childIter && childIter->hasNext()){
        childIter->goToNext();
        index++;
    } else {
        if (childIter) delete childIter;
        return (size_t)-1;
    }
    while (childIter && childIter->hasNext()){
        void *childData = childIter->getElement(leafSize);
        if (childData != nullptr){
            delete childIter;
            return index;
        }
        childIter->goToNext();
        index++;
    }
    delete childIter;
    return (size_t)-1;
}

bool Tree::Node::isLeaf(){
    if(children.empty())
        return true;
    List::ListIterator *childIter = static_cast<List::ListIterator*>(children.newIterator());
    while (childIter && childIter->hasNext()){
        void* childData = childIter->getElement(leafSize);
        Node* child = (childData) ? *(static_cast<Node**>(childData)) : nullptr;
        if (child != nullptr) {
            delete childIter;
            return false;
        }
        childIter->goToNext();
    }
    if (childIter) delete childIter;
    return true;
}

size_t Tree::Node::removeChildren(MemoryManager &mem){
    size_t numberOfDeletedElems = 0;
    List::ListIterator *childIter = static_cast<List::ListIterator*>(children.newIterator());
    while (childIter && childIter->hasNext()){
        void* childData = childIter->getElement(leafSize);
        Node* child = (childData) ? *(static_cast<Node**>(childData)) : nullptr;
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
    if (childIter) delete childIter;
    return numberOfDeletedElems;
}

Tree::Iterator *Tree:: createIterator(Tree:: Node *node){
    return new Tree::Iterator(&Root, node);
}