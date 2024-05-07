#include "derevo.h"
using namespace std;

int Tree::insert(Iterator *iter, int child_index, void *elem, size_t size){
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
    //return new TreeIterator(this, 0);
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
    return true;
}

bool Tree::TreeIterator::goToChild(int child_index){
    return true;
}

void* Tree::TreeIterator::getElement(size_t &size){
    listIterator->getElement(size);
}

bool Tree::TreeIterator::hasNext(){
    return listIterator->hasNext();
}

void Tree::TreeIterator::goToNext(){
    listIterator->goToNext();
}

bool Tree::TreeIterator::equals(Container::Iterator *right){
    return listIterator->equals(right);
}