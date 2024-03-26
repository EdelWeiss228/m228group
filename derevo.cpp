#include "derevo.h"
#include "List.h"
using namespace std;

typedef struct level {
    int color;
    list <list<void*>> child;
} level;


bool tree::TreeIterator::goToParent(){
    return true;
}

bool tree::TreeIterator::goToChild(int child_index){
    return true;
}

int tree::insert(Iterator *iter, int child_index, void *elem, size_t size){
    return 1;
}

bool tree::remove(Iterator *iter, int leaf_only){
    return true;
}

int tree::size(){
    return 0;
}

size_t tree::max_bytes(){
    return 0;
}

tree::Iterator* tree::find(void *elem, size_t size){
    return 0;
}

tree:: Iterator* tree::newIterator(){
    return 0;
}
void tree::remove(Container::Iterator *iter){
    //return;
}
void tree::clear(){
    //return;
}
bool tree::empty(){
    return true;
}
void* Container::Iterator::getElement(size_t &size){}
bool Container::Iterator::hasNext(){
    return true;
}
void Container::Iterator::goToNext(){}
bool Container::Iterator:: equals(Iterator *right){
    return true;
}