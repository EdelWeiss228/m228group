#include "derevo.h"
using namespace std;

Tree::Tree(MemoryManager &mem) : AbstractTree(mem), Root(nullptr) {}

Tree::~Tree() {
    clear();
}

void Tree::TreeIterator::resizeParentArray(){
    if (fullParentArrayCap == 0){
        fullParentArrayCap=1;
    }
    else{
        fullParentArrayCap=fullParentArrayCap*2;
    }
    parentArray = (ParentInfo*)realloc(parentArray, fullParentArrayCap*sizeof(ParentInfo));
}

int Tree::insert(Iterator *iter, int child_index, void *elem, size_t size){
    TreeIterator* treeIter = dynamic_cast<TreeIterator*>(iter);
    if(treeIter&&treeIter->curNode){
        
    }
    return 1;
}

bool Tree::remove(Iterator *iter, int leaf_only){
    return true;
}

int Tree::size(){
    return size();
}

size_t Tree::max_bytes(){
    return size();
}

Tree::Iterator* Tree::find(void *elem, size_t size){
    return 0;
}

Tree:: Iterator* Tree::newIterator(){
    if(Root){
        return new TreeIterator(this, Root, nullptr, 0);
    }
    return nullptr;
}

void Tree::remove(Container::Iterator *iter){

}

void Tree::clear(){

}

bool Tree::empty(){
    return true;
}

bool Tree::TreeIterator::goToParent(){
    Node* parent = findParent(curNode);
    if (parent){
        curNode = parent;
        return true;
    }
    return false;
}

bool Tree::TreeIterator::goToChild(int child_index){
    if(curNode&&curNode->children){
        List::Iterator* childIterator=curNode->children->newIterator();
        size_t curIndex=0;
        size_t elemSize=0;
        while(childIterator&&childIterator->hasNext()){
            if(curIndex==child_index){
                Node* childNode = static_cast <Node*>(childIterator->getElement(elemSize));
                addParentInfo(childNode, curNode);
                curNode = childNode;
                return true;
            }
            childIterator->goToNext();
            curIndex++;
        }
    }
    return false;
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