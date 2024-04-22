#include "derevo.h"
#include "List.h"
using namespace std;

int tree::insert(Iterator *iter, int child_index, void *elem, size_t size){
    return 1;
}

bool tree::remove(Iterator *iter, int leaf_only){
    return true;
}

int tree::size(){
    return size();
}

size_t tree::max_bytes(){
    return size();
}

tree::Iterator* tree::find(void *elem, size_t size){
    return 0;
}

tree:: Iterator* tree::newIterator(){
    return new TreeIterator();
}

void tree::remove(Container::Iterator *iter){

}

void tree::clear(){

}

bool tree::empty(){
    return true;
}

bool tree::TreeIterator::goToParent(){
    return true;
}

bool tree::TreeIterator::goToChild(int child_index){
    return true;
}

void* tree::TreeIterator::getElement(size_t &size){
    listIterator->getElement(size);
}

bool tree::TreeIterator::hasNext(){
    return listIterator->hasNext();
}

void tree::TreeIterator::goToNext(){
    listIterator->goToNext();
}

bool tree::TreeIterator::equals(Container::Iterator *right){
    return listIterator->equals(right);
}