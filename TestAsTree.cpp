#include "TestAsTree.h"

#include <cstring>
#include <iostream>

// public

void TestAsTree::run()
{
    const char *currentFunction = "TestAsTree::run()";

    std::cout << currentFunction << ": begin" << std::endl;
    if (false == insert())
    {
        std::cout << currentFunction << ": TestAsTree::insert() failed" << std::endl;
        _testResult = false;
        return;
    }
    std::cout << currentFunction << ": TestAsTree::insert() success" << std::endl;

    if (false == multiinsert())
    {
        std::cout << currentFunction << ": TestAsTree::multiinsert() failed" << std::endl;
        _testResult = false;
        return;
    }
    std::cout << currentFunction << ": TestAsTree::multiinsert() success" << std::endl;

    if (false == remove())
    {
        std::cout << currentFunction << ": TestAsTree::remove() failed" << std::endl;
        _testResult = false;
        return;
    }
    std::cout << currentFunction << ": TestAsTree::remove() success" << std::endl;

    std::cout << currentFunction << ": end" << std::endl;
}

bool TestAsTree::result()
{
    return _testResult;
}

// private

bool TestAsTree::insert()
{
    const char *currentFunction = "   TestAsTree::insert()";

    AbstractTree &tree = abstractTree();
    AbstractTree::Iterator *iterator =
        dynamic_cast<AbstractTree::Iterator *>(tree.newIterator());
    if (nullptr == iterator)
    {
        std::cout << currentFunction << ": it's impossible to convert iterator" << std::endl;
        return false;
    }

    // Проверка этапов вставки

    // Проверка вставки
    TestData data;
    data.i = 15;
    data.c[0] = 10;
    data.c[1] = 5;
    int insertionResult = tree.insert(iterator, 0, &data, sizeof(data));
    if (1 == insertionResult)
    {
        delete iterator;
        std::cout << currentFunction << ": insertion failed" << std::endl;
        return false;
    }

    // Переход к потомку, куда вставили данные
    bool goToChildResult = iterator->goToChild(0);
    if (false == goToChildResult)
    {
        delete iterator;
        std::cout << currentFunction << ": goToChild after insertion failed" << std::endl;
        return false;
    }

    // Проверка, что данные такие же, какие и вставили
    void *elementData = nullptr;
    size_t elementSize = 0;
    elementData = iterator->getElement(elementSize);
    if (sizeof(data) != elementSize) // размеры не совпадают
    {
        delete iterator;
        std::cout << currentFunction << ": size of data is not equal" << std::endl;
        return false;
    }
    if (0 != ::memcmp(elementData, &data, sizeof(data))) // данные не совпадают
    {
        delete iterator;
        std::cout << currentFunction << ": data is not equal" << std::endl;
        return false;
    }

    delete iterator;
    tree.clear();
    return true;
}

bool TestAsTree::multiinsert()
{
    const char *currentFunction = "   TestAsTree::multiinsert()";

    // Тестовые данные

    // для дочерних вершин первого слоя
    TestData testDataFirstLevel[5];
    for (size_t i = 0; i < 5; ++i)
    {
        testDataFirstLevel[i].i = 100 * (i + 1);
        testDataFirstLevel[i].c[0] = 10 * (i + 1);
        testDataFirstLevel[i].c[1] = 5 * (i + 1);
    }

    // для дочерних вершин второго слоя -- по три листа у каждого корня первого слоя
    TestData testDataSecondLevel[15];
    for (size_t i = 0; i < 15; ++i)
    {
        testDataSecondLevel[i].i = 200 * (i + 1);
        testDataSecondLevel[i].c[0] = 20 * (i + 1);
        testDataSecondLevel[i].c[1] = 10 * (i + 1);
    }
                                                    
    // Проверка вставки

    AbstractTree &tree = abstractTree();
    AbstractTree::Iterator *iterator =
        dynamic_cast<AbstractTree::Iterator *>(tree.newIterator());
    if (nullptr == iterator)
    {
        std::cout << currentFunction << ": it's impossible to convert iterator" << std::endl;
        return false;
    }

    // Заполнение первого слоя с самопроверкой
    for (size_t i = 0; i < 5; ++i)
    {
        // Проверка вставки
        int insertionResult = tree.insert(
            iterator, i, &testDataFirstLevel[i], sizeof(TestData));
        if (1 == insertionResult)
        {
            delete iterator;
            std::cout << currentFunction << ": insertion failed -- "
                      << "testDataFirstLevel[" << i << "]" << std::endl;
            return false;
        }

        // Переход к потомку, куда вставили данные
        bool goToChildResult = iterator->goToChild(i);
        if (false == goToChildResult)
        {
            delete iterator;
            std::cout << currentFunction << ": 1 level goToChild(" << i << ')'
                      << " after insertion failed" << std::endl;
            return false;
        }

        // Проверка, что данные такие же, какие и вставили
        void *elementData = nullptr;
        size_t elementSize = 0;
        elementData = iterator->getElement(elementSize);
        if (sizeof(testDataFirstLevel[i]) != elementSize) // размеры не совпадают
        {
            delete iterator;
            std::cout << currentFunction << ": 1 level size of data is not equal -- "
                      << "testDataFirstLevel[" << i << "]" << std::endl;
            return false;
        }
        if (0 != ::memcmp(elementData, &testDataFirstLevel[i], sizeof(testDataFirstLevel[i]))) // данные не совпадают
        {
            delete iterator;
            std::cout << currentFunction << ": 1 level data is not equal -- "
                      << "testDataFirstLevel[" << i << "]" << std::endl;
            return false;
        }

        // Переход к родитескому узлу
        bool goToParentResult = iterator->goToParent();
        if (false == goToParentResult)
        {
            delete iterator;
            std::cout << currentFunction << ": 1 level goToParent() failed" << std::endl;
            return false;
        }
    }

    // Заполнение второго слоя c самопроверкой
    for (size_t i = 0; i < 5; ++i)
    {
        bool goToChildResult = iterator->goToChild(i);
        if (false == goToChildResult)
        {
            delete iterator;
            std::cout << currentFunction << ": 2 level goToChild(" << i << ')' << std::endl;
            return false;
        }

        for (size_t j = 0; j < 3; ++j)
        {
            size_t index = (i * 3) + j;
            int insertionResult = tree.insert(
                iterator, j, &testDataSecondLevel[index], sizeof(TestData));
            if (1 == insertionResult)
            {
                delete iterator;
                std::cout << currentFunction << ": 2 level insertion failed -- "
                          << "testDataSecondLevel[" << index << "], i = " << i << ", j = " << j
                          << std::endl;
                return false;
            }

            // Переход к потомку, куда вставили данные
            bool goToChildResult = iterator->goToChild(j);
            if (false == goToChildResult)
            {
                delete iterator;
                std::cout << currentFunction << ": 2 level goToChild(" << j << ')'
                          << " after insertion failed" << std::endl;
                return false;
            }

            // Проверка, что данные такие же, какие и вставили
            void *elementData = nullptr;
            size_t elementSize = 0;
            elementData = iterator->getElement(elementSize);
            if (sizeof(testDataSecondLevel[index]) != elementSize) // размеры не совпадают
            {
                delete iterator;
                std::cout << currentFunction << ": 2 level size of data is not equal -- "
                          << "testDataSecondLevel[" << index << "], i = " << i << ", j = " << j
                          << std::endl;
                return false;
            }
            if (0 != ::memcmp(elementData, &testDataSecondLevel[index],
                              sizeof(testDataFirstLevel[index]))) // данные не совпадают(по байтово)
            {
                delete iterator;
                std::cout << currentFunction << ": 2 level data is not equal -- "
                          << "testDataSecondLevel[" << index << "], i = " << i << ", j = " << j
                          << std::endl;
                return false;
            }

            // Переход к родитескому узлу
            bool goToParentResult = iterator->goToParent();
            if (false == goToParentResult)
            {
                delete iterator;
                std::cout << currentFunction << ": 2 level goToParent() failed" << std::endl;
                return false;
            }
        }
        iterator->goToParent();
    }

    delete iterator;
    tree.clear();
    return true;
}

bool TestAsTree::remove()
{
    const char *currentFunction = "   TestAsTree::remove()";

    // Используем часть кода из multiinsert, но без проверок -- уже проверили

    // Для дочерних вершин первого слоя
    TestData testDataFirstLevel[5];
    for (size_t i = 0; i < 5; ++i)
    {
        testDataFirstLevel[i].i = 100 * (i + 1);
        testDataFirstLevel[i].c[0] = 10 * (i + 1);
        testDataFirstLevel[i].c[1] = 5 * (i + 1);
    }

    // Для дочерних вершин второго слоя -- по три листа у каждого корня первого слоя
    TestData testDataSecondLevel[15];
    for (size_t i = 0; i < 15; ++i)
    {
        testDataSecondLevel[i].i = 200 * (i + 1);
        testDataSecondLevel[i].c[0] = 20 * (i + 1);
        testDataSecondLevel[i].c[1] = 10 * (i + 1);
    }

    // Вставка

    AbstractTree &tree = abstractTree();
    AbstractTree::Iterator *iterator =
        dynamic_cast<AbstractTree::Iterator *>(tree.newIterator());

    for (size_t i = 0; i < 5; ++i)
        tree.insert(iterator, i, &testDataFirstLevel[i], sizeof(TestData));

    for (size_t i = 0; i < 5; ++i)
    {
        iterator->goToChild(i);
        for (size_t j = 0; j < 3; ++j)
        {
            size_t index = (i * 3) + j;
            tree.insert(iterator, j, &testDataSecondLevel[index], sizeof(TestData));
        }
        iterator->goToParent();
    }

    // Удаление

    // Варианты проверки удаления
    int leafOnly = 1;
    int notLeafOnly = 0;

    // Узел является листом, флаг 1
    iterator->goToChild(0); // первый слой, крайний левый
    iterator->goToChild(0); // второй слой слой, крайний левый
    bool resultRemoveLeafNodeWithLeafOnly = tree.remove(iterator, leafOnly);
    if (false == resultRemoveLeafNodeWithLeafOnly)
    {
        std::cout << currentFunction << ": Remove leaf with leafOnly flag failed" << std::endl;
        return false;
    }

    // Узел не является листом, флаг 1
    iterator->goToParent(); // первый слой, крайний левый
    bool resultRemoveNoLeafNodeWithLeafOnly = tree.remove(iterator, leafOnly);
    if (true == resultRemoveNoLeafNodeWithLeafOnly)
    {
        std::cout << currentFunction << ": Remove 'no leaf' with leafOnly flag failed" << std::endl;
        return false;
    }

    // Узел является листом, флаг 0
    iterator->goToChild(0); // второй слой слой, крайний левый
    bool resultRemoveLeafNodeWithoutLeafOnly = tree.remove(iterator, leafOnly);
    if (false == resultRemoveLeafNodeWithoutLeafOnly)
    {
        std::cout << currentFunction << ": Remove leaf without leafOnly flag failed" << std::endl;
        return false;
    }

    // Узел не является листом, флаг 0
    iterator->goToParent(); // первый слой, крайний левый
    bool resultRemoveNoLeafNodeWithoutLeafOnly = tree.remove(iterator, notLeafOnly);
    if (false == resultRemoveNoLeafNodeWithoutLeafOnly)
    {
        std::cout << currentFunction << ": Remove 'no leaf' with leafOnly flag failed" << std::endl;
        return false;
    }

    delete iterator;
    tree.clear();
    return true;
}
// Реалиацзия теста
/*
количество элементов 10 000 аргументом
два ребенка у родителя
реализация сбалансированного дерева 
std::chrono
найти каждый элемент(поиск не существующих элементов)::chrono
в цикле удаляем все четные элементы
remove не на целое поддерево.

std::chrono пройтись по дереву что лежат только нечетные элементы std::chrono

вывод 4 времени
время на добавление, поиск, удаление и проход
на вход парамет n - количество
*/