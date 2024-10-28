#include "derevo.h"
using namespace std;

Tree::Node* Tree::Iterator::returnNode(){
    size_t objectSize=0;
    if (currentIterator == nullptr)
        return nullptr;
    void* tmpElem = this->currentIterator->getElement(objectSize);
    return static_cast<Node*>(tmpElem);
}

int Tree::insert(AbstractTree::Iterator* iter, int child_index, void* elem, size_t size) {
    std::cout << "Начало функции insert." << std::endl;
    Iterator* tmp = static_cast<Iterator*>(iter);
    bool emptinessFlag = true;

    // Проверяем наличие итератора и состояния дерева
    
        if (tmp == nullptr) {
        std::cout << "Итератор не инициализирован, создаем новый." << std::endl;
        tmp = this->newIterator(); // Инициализация нового итератора
        if (tmp == nullptr) {
            std::cout << "Ошибка: не удалось создать новый итератор." << std::endl;
            return 1; // Ошибка
        }
    }


    if (!tmp) {
        std::cout << "Итератор не инициализирован." << std::endl;
        if (empty()) {
            std::cout << "Дерево пустое, невозможно вставить элемент." << std::endl;
            return 1;
        }
        emptinessFlag = false;
    }

    if (this->empty()) {
    std::cout << "Дерево пустое, создаем новый итератор для корня." << std::endl;
    // Здесь создайте новый итератор для корня
    tmp = newIterator();
    if (tmp == nullptr) {
        std::cout << "Ошибка: не удалось создать итератор для корня." << std::endl;
        return 1; // Возвращаем ошибку
    }
}



    Node* tmpNode = nullptr;
    List* tmpList = nullptr;

    // Определяем узел родителя, если дерево не пустое
    if (!emptinessFlag) {
        tmpNode = tmp->returnNode();
        std::cout << "Текущий узел перед вызовом getElement: " << tmpNode << std::endl;

        if (!tmpNode) {
            std::cout << "Ошибка: указатель tmpNode равен nullptr." << std::endl;
            return 1;
        }
        std::cout << "Узел найден, родительский узел: " << tmpNode << std::endl;
        tmpList = tmpNode->children;
    } else {
        tmpList = this->Root;
        std::cout << "Работаем с корнем дерева." << std::endl;
    }

    if (!tmpList) {
        std::cout << "Ошибка: указатель tmpList равен nullptr." << std::endl;
        return 1;
    }

    // Выделяем память для нового дочернего списка и нового объекта
    void* tmpChildren = _memory.allocMem(sizeof(List));
    if (!tmpChildren) {
        std::cout << "Ошибка: не удалось выделить память для дочернего списка." << std::endl;
        return 1;
    }

    void* tmpObject = _memory.allocMem(size);
    if (!tmpObject) {
        std::cout << "Ошибка: не удалось выделить память для объекта." << std::endl;
        _memory.freeMem(tmpChildren);
        return 1;
    }

    // Инициализируем новый список
    new (tmpChildren) List(this->_memory);
    std::cout << "Новый дочерний список успешно создан." << std::endl;

    // Создаем новый узел с выделенной памятью
    Node* newObject = new Node {tmpObject, static_cast<List*>(tmpChildren), size, tmpNode};
    if (!newObject) {
        std::cout << "Ошибка: не удалось создать новый узел." << std::endl;
        _memory.freeMem(tmpChildren);
        _memory.freeMem(tmpObject);
        return 1;
    }

    // Копируем содержимое элемента
    memcpy(tmpObject, elem, size);
    std::cout << "Элемент успешно скопирован в память." << std::endl;

    // Вставляем новый узел в список дочерних элементов
    tmpList->push_front(newObject, sizeof(*newObject));
    std::cout << "Элемент успешно вставлен в список дочерних элементов." << std::endl;

    // Увеличиваем количество элементов
    NumberOfElems++;
    std::cout << "Текущий размер дерева: " << NumberOfElems << std::endl;

    return 0;
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
    Iterator* treeIter = this->newIterator();
    if (treeIter){
        Node* tmpNode = treeIter->returnNode();
        if (tmpNode){
            bool searchFlag = true;
            Node* rootNode = tmpNode;
            do{
                if (searchFlag && treeIter->goToChild(0)) 
                    tmpNode = treeIter->returnNode();
                else if (treeIter->hasNext()){
                    treeIter->goToNext();
                    tmpNode = treeIter->returnNode();
                    searchFlag = true;
                }
                else if (tmpNode->size==size && searchFlag){
                    unsigned char* leftBytes = static_cast<unsigned char*>(tmpNode->leaf);
                    unsigned char* rightBytes = static_cast<unsigned char*>(elem);
                    if (memcmp (leftBytes, rightBytes, size)==0);
                        return treeIter;
                }
                else{
                    treeIter->goToParent();
                    tmpNode = treeIter->returnNode();
                    searchFlag = false;
                }
                
            } while (rootNode != tmpNode);
        }
        delete treeIter;
    }
    return nullptr;
}

Tree::Iterator* Tree::newIterator() {
    if (this->Root == nullptr) {
        return nullptr; // Если корневой список пуст, возвращаем nullptr
    }

   Iterator* newIter = new Iterator();
newIter->currentIterator = dynamic_cast<List::Iterator*>(this->Root->newIterator());
newIter->parentIterator = nullptr; // Set parent if necessary
return newIter;

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
if (curNode == nullptr) {
    size = 0;
    return nullptr; // Или обработайте ошибку
}
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
