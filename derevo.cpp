#include "derevo.h"
#include "queue"
using namespace std;

Tree::Tree(MemoryManager &mem) : AbstractTree(mem), Root(nullptr) {}

Tree::~Tree() {clear();}

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
        Node* newNode = new Node(elem, _memory);
        if (!newNode) return 1;
        treeIter->curNode->children->insert(treeIter->listIterator, elem, size);
        return 0;
    }
    return 1;
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
    if(!treeIter) return false;
    size_t size;
    Node* targetNode = treeIter->curNode;
    Node* parentNode = treeIter->findParent(targetNode);
    if (leaf_only==1&&targetNode->children->size()>0)
        return false;
    if(parentNode){
        List::Iterator* childIterator = parentNode->children->newIterator();
        while(childIterator&&childIterator->hasNext()){
            Node* childNode = static_cast<Node*>(childIterator->getElement(size));
            if(childNode == targetNode){
                parentNode->children->remove(childIterator);
                break;
            }
            childIterator ->goToNext();
        }
    } else if (Root == targetNode){
        Root == nullptr;
    }
    deleteSubtree(targetNode);
    return true;
}

int Tree::size(){
    return size(); 
}

size_t Tree::max_bytes(){
    return size();
}

AbstractTree::Iterator* Tree::find(void *elem, size_t size){
    if (!Root) return nullptr;
    struct ParentInfo
    {
        Node* node1;
        Node* node2;
    };
    
    queue <ParentInfo> queue;
    ParentInfo rootPair = {nullptr, Root};
    queue.push(rootPair);
    while (!queue.empty()) {
        auto [parent, curNode] = queue.front();
        queue.pop();

        if(memcmp(curNode->leaf, elem, size)==0){
            TreeIterator* iterator = new TreeIterator(this, curNode, 0);
            Node* tmp = parent;
            while (tmp){
                iterator->addParentInfo(curNode, tmp);
                curNode = tmp;
                tmp = iterator->findParent(tmp);
            }
            return iterator;
        }
        List::Iterator* childIter = curNode->children->newIterator();
        size_t size =0;
        while(childIter&&childIter->hasNext()){
            Node* childNode = static_cast<Node*>(childIter->getElement(size));
            ParentInfo pair ={curNode, childNode};
            queue.push(pair);
            childIter->goToNext();
        }
    }
    return nullptr;
}

Tree:: Iterator* Tree::newIterator(){
    if(Root){
        return new TreeIterator(this, Root, 0);
    }
    return nullptr;
}

void Tree::remove(Container::Iterator *iter){
    TreeIterator* treeIter = dynamic_cast<TreeIterator*>(iter);
    if(!treeIter) return;
    Node* targetNode = treeIter->curNode;
    Node* parentNode = treeIter->findParent(targetNode);
    size_t size=0;
    if(parentNode){
        List::Iterator* childIter = parentNode ->children->newIterator();
        while (childIter&& childIter->hasNext()){
            Node* childNode = static_cast<Node*>(childIter->getElement(size));
            if(childNode==targetNode){
                parentNode->children->remove(childIter);
                break;
            }
            childIter->goToNext();
        }
    }
    else if (Root == targetNode) {
        Root == nullptr;
    }
    deleteSubtree(targetNode);
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
List::Iterator* Tree::newListIterator(Node* node, size_t& listPosition, bool toBegin){
    return node->children->newIterator();
}