#include "Tree.h"
#include <iostream>
#include <limits>   

using std::cout;
using std::endl;

// Constructors and destuctors

Tree::Tree(MemoryManager &manager) : AbstractTree(manager), _root{manager}, _size{0} {}

Tree::~Tree()
{
    // переделать деструктор, потому что есть элементы с nullptr
    _root.removeChildren(this->_memory);
}

Tree::Node::Node(MemoryManager &manager) : children(manager) {}
// автоматические сгенерируется
Tree::Node::~Node() {}

Tree::TreeIterator::TreeIterator(const Node *root, Node *node) : _root{root}, _node{node} {}

// From container

void *Tree::TreeIterator::getElement(size_t &amount)
{
    if (nullptr == _node)
        return nullptr;

    amount = _node->dataSize;
    return _node->dataValue;
}

int Tree::size()
{
    return _size;
}

size_t Tree::max_bytes()
{
    return std::numeric_limits<std::size_t>::max();
}

bool Tree::empty()
{
    return size() == 0;
}

void Tree::clear()
{
    if (empty())
        return;
}

// From AbstractTree

int Tree::insert(AbstractTree::Iterator *iter, int child_index, void *elem, size_t size)
{
    if (child_index >= 0)
    {
        TreeIterator *inputIterator = nullptr;
        try
        {
            inputIterator = convert(iter);
        }
        catch (const Container::Error &error)
        {
            std::cerr << "Tree::insert(): cannot convert iterator: error = '"
                      << error.msg << "'" << std::endl;
            return 1;
        }
        // Переделать под вывод Container
        Node *tmpNode = inputIterator->getCurrentNode();
        if (nullptr == tmpNode)
            throw std::invalid_argument{"Invalid parameter: iterator is null"};

        List::ListIterator *iter_children = static_cast<List::ListIterator *>(tmpNode->children.newIterator());
        int current_index = 0;
        for (int i = 0; i < child_index; ++i)
        {
            if (!iter_children->hasNext())
            {
                Node *emptyNode = static_cast<Node *>(this->_memory.allocMem(sizeof(Node)));
                new (emptyNode) Node{this->_memory};

                tmpNode->children.push_front(emptyNode, sizeof(Node));
            }
            iter_children->goToNext();
        }

        Node *newNode = static_cast<Node *>(this->_memory.allocMem(sizeof(Node)));
        new (newNode) Node{this->_memory};

        newNode->parent = tmpNode;
        newNode->dataValue = this->_memory.allocMem(size);
        ::memcpy(newNode->dataValue, elem, size);
        newNode->dataSize = size;

        tmpNode->children.insert(iter_children, newNode, sizeof(Node));

        ++_size;

        return 0;
    }
    return 1;
}

Container::Iterator *Tree::newIterator()
{
    return createIterator(_root.getFirst());
}
// переделать
Tree::Node *Tree::Node::getFirst()
{
    if (children.empty())
        return this;

    List::ListIterator *iter_children = static_cast<List::ListIterator *>(children.newIterator());
    while (iter_children->hasNext())
    {
        Node *child = static_cast<Node *>(iter_children->getElement(dataSize));
        if (nullptr != child)
            return child->getFirst();
        iter_children->goToNext();
    }
    return this;
}

bool Tree::TreeIterator::hasNext()
{
    if ((nullptr == _node) || (_root == _node))
        return false;

    size_t index = _node->parent->indexOfNextChild(_node);
    if ((_root == _node->parent) && (std::numeric_limits<size_t>::max() == index))
        return false;

    return true;
}

void Tree::TreeIterator::goToNext()
{
    if (!hasNext())
        return;

    List::ListIterator *iter_children = static_cast<List::ListIterator *>(_node->parent->children.newIterator());
    size_t index = 0;
    bool found = false;

    while (iter_children->hasNext())
    {
        void *child_elem = iter_children->getElement(_node->dataSize);
        if (static_cast<Node *>(child_elem) == _node)
        {
            found = true;
            break;
        }
        iter_children->goToNext();
        ++index;
    }

    if (!found || iter_children->hasNext())
    {
        iter_children->goToNext();
        ++index;
    }

    if (!iter_children->hasNext())
    {
        _node = _node->parent;
    }
    else
    {
        void *child_elem = iter_children->getElement(_node->dataSize);
        Node *next_child = static_cast<Node *>(child_elem);
        _node = next_child->getFirst();
    }
}

bool Tree::TreeIterator::goToParent()
{
    if (nullptr == _node)
        throw std::invalid_argument{"Invalid parameter: iterator is null"};

    if (nullptr == _node->parent)
        return false;

    Node *parent = _node->parent;
    _node = parent;
    return true;
}

bool Tree::TreeIterator::goToChild(int child_index)
{
    if (nullptr == _node)
        throw std::invalid_argument{"Invalid parameter: iterator is null"};

    int current_index = 0;
    List::ListIterator *iter_children = static_cast<List::ListIterator *>(_node->children.newIterator());

    while (iter_children->hasNext())
    {
        if (current_index == child_index)
        {
            void *child_elem = iter_children->getElement(_node->dataSize);
            Node *child = static_cast<Node *>(child_elem);
            _node = child;
            return true;
        }
        iter_children->goToNext();
        ++current_index;
    }

    return false;
}

size_t Tree::Node::indexOfNextChild(Node *node)
{
    size_t index = 0;

    List::ListIterator *iter_children = static_cast<List::ListIterator *>(children.newIterator());

    // Ищем текущий узел
    while (iter_children->hasNext())
    {
        void *elem = iter_children->getElement(dataSize);
        if (static_cast<Node *>(elem) == node)
            break;
        iter_children->goToNext();
        ++index;
    }

    // Пропускаем текущий узел
    if (iter_children->hasNext())
    {
        iter_children->goToNext();
        ++index;
    }

    // Ищем следующий узел
    while (iter_children->hasNext())
    {
        void *elem = iter_children->getElement(dataSize);
        if (static_cast<Node *>(elem) != nullptr)
            return index;
        iter_children->goToNext();
        ++index;
    }
    // добавить удаление итераторов

    // Если следующий узел не найден
    return std::numeric_limits<size_t>::max();
}

bool Tree::Node::isLeaf()
{
    if (children.empty())
        return true;
    List::ListIterator *iter_children = static_cast<List::ListIterator *>(children.newIterator());
    while (iter_children->hasNext())

    {
        void *child_elem = iter_children->getElement(dataSize);
        Node *child = static_cast<Node *>(child_elem);
        if (nullptr != child)
            return false;
    }
    return true;
}

size_t Tree::Node::removeChildren(MemoryManager &manager)
{
    size_t amountOfRemoved = 0;
    List::ListIterator *iter_children = static_cast<List::ListIterator *>(children.newIterator());

    while (iter_children->hasNext())
    {
        void *child_elem = iter_children->getElement(dataSize);
        Node *child = static_cast<Node *>(child_elem);
        if (child == nullptr)
        {
            iter_children->goToNext();
            continue;
        }

        amountOfRemoved += child->removeChildren(manager);
        if (child->dataValue)
        {
            manager.freeMem(child->dataValue);
            child->dataValue = nullptr;
            child->dataSize = 0;
        }
        child->~Node();
        manager.freeMem(child);
        ++amountOfRemoved;

        children.remove(iter_children);
    }
    return amountOfRemoved;
}

// Отредактировать метод. Сначала проинициализировать 4 метода
bool Tree::remove(AbstractTree::Iterator *iter, int leaf_only)
{
    if (nullptr == iter)
        throw std::invalid_argument{"Invalid parameter: iterator is null"};

    TreeIterator *inputIterator = nullptr;
    try
    {
        inputIterator = convert(iter);
    }
    catch (const std::runtime_error &error)
    {
        std::cerr << "Tree::remove(): cannot convert iterator: error = '"
                  << error.what() << "'" << std::endl;
        return false;
    }

    if (empty() || (&_root == inputIterator->getCurrentNode()))
        return false;

    Node *node = inputIterator->getCurrentNode();
    if (nullptr == node)
        throw std::invalid_argument{"Invalid parameter: node is null"};

    bool isLeaf = node->isLeaf();
    if ((1 == leaf_only) && (false == isLeaf))
        return false;

    // Даже если удалится узел с потомками, итератор перескочит на соседний
    bool hasNext = inputIterator->hasNext();
    if (hasNext)
        inputIterator->goToNext();

    if (!isLeaf)
        _size -= node->removeChildren(_memory);

    // Ищем этого потомка в узле выше
        List::ListIterator *iter_children = static_cast<List::ListIterator *>(node->parent->children.newIterator());
    bool found = false;

    while (iter_children->hasNext())
    {
        void *child_elem = iter_children->getElement(node->dataSize);
        if (static_cast<Node *>(child_elem) == node)
        {
            node->parent->children.remove(iter_children);
            found = true;
            break;
        }
        iter_children->goToNext();
    }

    if (!found)
        throw std::invalid_argument{"Invalid parameter: node not found in parent's children"};

    if (node->dataValue)
    {
        _memory.freeMem(node->dataValue);
        node->dataValue = nullptr;
        node->dataSize = 0;
    }
    node->~Node();
    _memory.freeMem(node);
    --_size;

    // Если удалили последний элемент, ищем другой последний элемент
    if (!hasNext)
    {
        List::ListIterator *iter_root_children = static_cast<List::ListIterator *>(_root.children.newIterator());
        while (iter_root_children->hasNext())
        {
            void *elem = iter_root_children->getElement(node->dataSize);
            Node *child = static_cast<Node *>(elem);
            if (child != nullptr)
            {
                new (inputIterator) TreeIterator{&_root, child}; // пересоздаём итератор по адресу старого
                break;
            }
            iter_root_children->goToNext();
        }
    }

    return true;
}

void Tree::remove(Container::Iterator *iter)
{
    Iterator *inputIterator = nullptr;
    try
    {
        inputIterator = convert(iter);
    }
    catch (const Container::Error &error)
    {
        std::cerr << "Tree::remove(iter): cannot convert iterator: error = '"
                  << error.msg << "'" << std::endl;
    }
    remove(inputIterator, 0);
}

Container::Iterator *Tree::find(void *elem, size_t size)
{
    if (0 == _size) // если контейнер пуст
        return nullptr;

    if ((nullptr == elem) || (0 == size))
        throw std::invalid_argument{"Invalid parameter: iterator is null"};

    Node *node = _root.find(elem, size);
    if (nullptr == node)
        return nullptr;

    return createIterator(node);
}

Tree::Node *Tree::Node::find(void *elem, size_t size)
{
    if (dataSize == size && memcmp(dataValue, elem, size) == 0)
        return this;

    List::ListIterator *iter_children = static_cast<List::ListIterator *>(children.newIterator());

    while (iter_children->hasNext())
    {
        void *child_elem = iter_children->getElement(dataSize);
        Node *child = static_cast<Node *>(child_elem);
        if (child != nullptr)
        {
            Node *found = child->find(elem, size);
            if (found != nullptr)
            {
                return found;
            }
        }
        iter_children->goToNext();
    }

    // добавить очистку старых ненужных указателей

    return nullptr;
}

// Переписать под вывод ошибки для Container
Tree::TreeIterator *Tree::convert(Container::Iterator *iter)
{
    if (nullptr == iter)
        throw std::invalid_argument{"Invalid parameter: iterator is null"};
    Tree::TreeIterator *inputIter = dynamic_cast<Tree::TreeIterator *>(iter);
    if (nullptr == inputIter)
        throw std::invalid_argument{"Invalid iterator: dynamic_cast failed"};
    return inputIter;
}

Tree::TreeIterator *Tree::createIterator(Tree::Node *node)
{
    return new Tree::TreeIterator(&_root, node);
}
// удаление итератора, создание нового

bool Tree::TreeIterator::equals(Container::Iterator *right)
{
    TreeIterator *inputIter = dynamic_cast<TreeIterator *>(right);
    if (nullptr == inputIter)
        return false;
    return _node == inputIter->_node;
}
