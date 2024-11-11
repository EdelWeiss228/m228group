#pragma once

#include "TreeAbstract.h"
#include "LinkedList1.hpp"

class Tree : public AbstractTree
{
public:
    Tree(MemoryManager &manager);
    ~Tree();

public:
    struct Node
    {
        Node(MemoryManager &manager);
        ~Node();
        // создать конструктор для ноды для памяти List
        List children;
        Node *parent = nullptr;

        void *dataValue = nullptr;
        size_t dataSize = 0;

        size_t removeChildren(MemoryManager &manager); // complete

        // является ли узел листом
        bool isLeaf(); // complete

        // Найти узел по данным
        Node *find(void *data, size_t size); // complete

        // получаем самый крайний левый узел
        Node *getFirst();

        size_t indexOfNextChild(Node *node); // complete
    };

public:
    // From Container
    int size() override;                                         // complete
    size_t max_bytes() override;                                 // complete
    Container::Iterator *find(void *elem, size_t size) override; // complete
    Container::Iterator *newIterator() override;                 // complete
    void remove(Container::Iterator *iter) override;             // complete
    void clear() override;                                       // complete
    bool empty() override;                                       // complete

    // From AbstractTree
    int insert(AbstractTree::Iterator *iter, int child_index, void *elem, size_t size) override; // complete

    bool remove(AbstractTree::Iterator *iter, int leaf_only) override; // complete

public: // Methods of Iterator
    class TreeIterator : public AbstractTree::Iterator
    {
    public:
        TreeIterator(const Node *root, Node *node);
        ~TreeIterator() = default;

    public:
        // From Abstact::Itertator
        bool goToParent() override;               // complete
        bool goToChild(int child_index) override; // complete

        // From Container::Iterator
        void *getElement(size_t &size) override;          // complete
        bool hasNext() override;                          // complete
        void goToNext() override;                         // complete
        bool equals(Container::Iterator *right) override; // complete

        // геттер для получения текущей ноды
        Node *getCurrentNode() { return _node; } // complete

    private:
        const Node *_root;
        Node *_node;
    };

private:
    // Преобразовать указатель на итератор абстрактного контейнера в указатель на реализацию итератора дерева
    // Если указатель nullptr, выбросится исключение InvalidParameters
    // Если этого не удалось сделать, выбросится исключение InvalidIterator
    TreeIterator *convert(Container::Iterator *iter); // complete

    // Создать итератор с указателем на конкретную вершину
    TreeIterator *createIterator(Node *node); // complete

    Node _root;
    int _size;
public:
    class TestClass{

    };
};

// class Exceptions : public Container::Error
// {
// public:
//     class NotAvailable
//     {
//     public:
//         NotAvailable() : Container::Error{"Element is not available"} {}
//     };
//     class InvalidIterator
//     {
//     public:
//         InvalidIterator() : Container
//     };
//     class InvalidParameters
//     {
//     };
// };

// вставка по индексу - доходя до необходимого, создавать пустые элементы

// Реализовать исключения
// class Tree::NotAvailable : public Container::Error
// {
// public:
//     NotAvailable() : Container::Error{"Element is not available"} {}
// };

// class Tree::InvalidIterator : public Container::Error
// {
// public:
//     InvalidIterator() : Container::Error{"Iterator is not valid"} {}
// };

// class Tree::InvalidParameters : public Container::Error
// {
// public:
//     InvalidParameters() : Container::Error{"Paramter is not valid"} {}
// };
