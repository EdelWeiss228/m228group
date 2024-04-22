#pragma once
#include "GroupContainer.h"

// Абстрактный класс: дерево
class AbstractTree: public GroupContainer
{
public:
    // конструктор
    AbstractTree(MemoryManager &mem): GroupContainer(mem) {}

    // деструктор
    virtual ~AbstractTree() {}

    class Iterator: public Container::Iterator
    {
    public:
        // Переход к родительской вершине. Возвращает false если текущая вершина - корень или end().
        virtual bool goToParent() = 0;

        // Переход к дочерней вершине № child_index. Возвращает false если дочерней вершины с таким номером нет.
        virtual bool goToChild(int child_index) = 0;
    };

    // Добавление элемента как дочернюю вершину № child_index вершины, на которую
    // указывает итератор. В случае успешного добавления функция возвращает значение 0, в случае неудачи 1.
    virtual int insert(Iterator *iter, int child_index, void *elem, size_t size) = 0;

    // Удаление вершины, на которую указывает итератор. Если leaf_only==1 и вершина не является листом, возвращает false
    // !!! Примечание: метод remove удаляет вершину вместе со всеми ее потомками
    virtual bool remove(Iterator *iter, int leaf_only) = 0;
};