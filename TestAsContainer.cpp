#include "TestAsContainer.h"

#include <cstring>
#include <iostream>

void TestAsContainer::run()
{
    const char *currentFunction = "TestAsContainer::run()";

    std::cout << currentFunction << ": begin" << std::endl;

    if (false == size())
    {
        std::cout << currentFunction << ": TestAsContainer::size() failed" << std::endl;
        _testResult = false;
        return;
    }
    std::cout << currentFunction << ": TestAsContainer::size() success" << std::endl;

    if (false == max_bytes())
    {
        std::cout << currentFunction << ": TestAsContainer::max_bytes() failed" << std::endl;
        _testResult = false;
        return;
    }
    std::cout << currentFunction << ": TestAsContainer::max_bytes() success" << std::endl;

    if (false == find())
    {
        std::cout << currentFunction << ": TestAsContainer::find() failed" << std::endl;
        _testResult = false;
        return;
    }
    std::cout << currentFunction << ": TestAsContainer::find() success" << std::endl;

    if (false == newIterator())
    {
        std::cout << currentFunction << ": TestAsContainer::newIterator() failed" << std::endl;
        _testResult = false;
        return;
    }
    std::cout << currentFunction << ": TestAsContainer::newIterator() success" << std::endl;

    if (false == remove())
    {
        std::cout << currentFunction << ": TestAsContainer::remove() failed" << std::endl;
        _testResult = false;
        return;
    }
    std::cout << currentFunction << ": TestAsContainer::remove() success" << std::endl;

    if (false == traverse())
    {
        std::cout << currentFunction << ": TestAsContainer::traverse() failed" << std::endl;
        _testResult = false;
        return;
    }
    std::cout << currentFunction << ": TestAsContainer::traverse() success" << std::endl;

    std::cout << currentFunction << ": end" << std::endl;
}

bool TestAsContainer::result()
{
    return _testResult;
}

// private

// Ниже контейнер заполняется следующим образом
// номера -- индекс при проходе по дереву:
//
//       -------------- root -----------------
//      /         /       |        \          \
//     /         /        |         \          \
//    4         8        12         16         20
//  / | \     / | \     / | \      / | \      / | \
// 1  2  3   5  6  7   9 10 11   13 14 15   17 18 19
//
void TestAsContainer::fillContainer()
{
    // Для дочерних вершин первого слоя -- пять элементов
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
        testDataSecondLevel[i].i = 1000 * (i + 1);
        testDataSecondLevel[i].c[0] = 100 * (i + 1);
        testDataSecondLevel[i].c[1] = 10 * (i + 1);
    }

    // Вставка

    AbstractTree &tree = abstractTree();
    AbstractTree::Iterator *iterator =
        dynamic_cast<AbstractTree::Iterator *>(tree.newIterator());
    // Преобразуем от базового класса к производному классу

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

    delete iterator;
}

bool TestAsContainer::size()
{
    const char *currentFunction = "    TestAsContainer::size()";

    Container &container = this->container();
    if (0 != container.size())
    {
        std::cout << currentFunction
                  << ": container.size() with empty container got non-zero value"
                  << std::endl;
        return false;
    }
    fillContainer();
    if (20 != container.size())
    {
        std::cout << currentFunction
                  << ": container.size() with non-empty container got invalid value"
                  << std::endl;
        return false;
    }

    container.clear();
    return true;
}

bool TestAsContainer::max_bytes()
{
    const char *currentFunction = "    TestAsContainer::max_bytes()";
    if (std::numeric_limits<size_t>::max() != container().max_bytes())
    {
        std::cout << currentFunction
                  << ": container.max_bytes() got invalid value"
                  << std::endl;
        return false;
    }
    return true;
}

bool TestAsContainer::find()
{
    const char *currentFunction = "    TestAsContainer::find()";

    Container &container = this->container();
    // возвращаем ссылку на контейнер(ссылку на дерево, которое становится контейнером)
    // переходим от дочернего класса к предку

    // Существующее
    TestData existData;
    existData.i = 2000;
    existData.c[0] = 200;
    existData.c[1] = 20;

    // Несущетвующее
    TestData notExistData;
    notExistData.i = 2000;
    notExistData.c[0] = 200;
    notExistData.c[1] = 200;

    // Поиск в пустом контейнере
    Container::Iterator *iterator = container.find(&existData, sizeof(TestData));

    if (nullptr != iterator)
    {
        std::cout << currentFunction
                  << ": Empty container after find() didn't return nullptr" << std::endl;
        return false;
    }

    fillContainer(); // Заполняем контейнер

    // Поиск существующего в заполненном контейнере
    iterator = container.find(&existData, sizeof(TestData));
    if (nullptr == iterator)
    {
        std::cout << currentFunction
                  << ": Filled container after find(existData) didn't return iterator" << std::endl;
        return false;
    }
    size_t dataSize = 0;
    void *dataValue = iterator->getElement(dataSize);
    if ((sizeof(TestData) != dataSize) || (dataValue == nullptr) ||
        (0 != ::memcmp(&existData, dataValue, sizeof(TestData))))
    {
        delete iterator;
        std::cout << currentFunction
                  << ": Iterator returned invalid data" << std::endl;
        return false;
    }
    delete iterator;

    // Поиск несуществующего в заполненном контейнере
    iterator = container.find(&notExistData, sizeof(TestData));
    if (nullptr != iterator)
    {
        std::cout << currentFunction
                  << ": Filled container after find(notExistData) didn't return nullptr"
                  << std::endl;
        return false;
    }

    container.clear();
    return true;
}

bool TestAsContainer::newIterator()
{
    const char *currentFunction = "    TestAsContainer::newIterator()";

    Container &container = this->container();

    // Получить итератор пустого контейнера
    Container::Iterator *iteratorFromEmptyContainer = container.newIterator();
    if (true == iteratorFromEmptyContainer->hasNext())
    {
        delete iteratorFromEmptyContainer;
        std::cout << currentFunction << ": Iterator from empty container can traversing. It's wrong"
                  << std::endl;
        return false;
    }
    delete iteratorFromEmptyContainer;

    fillContainer();

    // Получить итератор непустого контейнера и проверить, что значение там то
    Container::Iterator *iterator = container.newIterator();
    // Первый ли элемент?
    TestData first;
    first.i = 1000;
    first.c[0] = 100;
    first.c[1] = 10;
    size_t dataSize = 0;
    void *dataValue = iterator->getElement(dataSize);
    if ((dataValue == nullptr) || (dataSize != sizeof(TestData)))
    {
        delete iterator;
        std::cout << currentFunction << ": Iterator doesn't have any data (first)"
                  << std::endl;
        return false;
    }
    if (0 != ::memcmp(dataValue, &first, sizeof(TestData)))
    {
        delete iterator;
        std::cout << currentFunction << ": First data doesn't equal iterator's data"
                  << std::endl;
        return false;
    }

    // Есть ли следующий?
    if (false == iterator->hasNext())
    {
        delete iterator;
        std::cout << currentFunction << ": Iterator doesn't have next node"
                  << std::endl;
        return false;
    }

    // Переходим на следующий
    iterator->goToNext();

    // Совпадает ли следующий?
    TestData second;
    second.i = 2000;
    second.c[0] = 200;
    second.c[1] = 20;
    dataValue = iterator->getElement(dataSize);
    if ((dataValue == nullptr) || (dataSize != sizeof(TestData)))
    {
        delete iterator;
        std::cout << currentFunction << ": Iterator doesn't have any data (second)"
                  << std::endl;
        return false;
    }
    if (0 != ::memcmp(dataValue, &second, sizeof(TestData)))
    {
        delete iterator;
        std::cout << currentFunction << ": Seond data doesn't equal iterator's data"
                  << std::endl;
        return false;
    }

    delete iterator;
    container.clear();
    return true;
}

bool TestAsContainer::remove()
{
    const char *currentFunction = "    TestAsContainer::remove()";

    Container &container = this->container();

    // Удалить узел из пустого контейнера
    Container::Iterator *iteratorFromEmptyContainer = container.newIterator();
    container.remove(iteratorFromEmptyContainer); // Ничего не будет просходить
    delete iteratorFromEmptyContainer;

    fillContainer();
    Container::Iterator *iterator = container.newIterator();
    size_t size = container.size();

    // Удалить первый
    container.remove(iterator);
    size_t currentSize = container.size();
    if (currentSize != (size - 1))
    {
        std::cout << currentFunction << ": First node didn't remove" << std::endl;
        return false;
    }

    // Удалить средние
    for (size_t i = 0; i < 4; ++i, iterator->goToNext()) // по старой структуре 5-й элемент
    {
    }
    container.remove(iterator);
    currentSize = container.size();
    if (currentSize != (size - 2))
    {
        return false;
    }
    for (size_t i = 0; i < 5; ++i, iterator->goToNext()) // по старой структуре 12-й элемент
    {
    }
    container.remove(iterator);
    currentSize = container.size();
    if (currentSize != (size - 6))
    {
        return false;
    }

    // Удалить последний
    for (; iterator->hasNext(); iterator->goToNext())
    {
    }
    // Последний узел не является листом, и удалён весь узел с дочерними узлами
    // Итератор будет указывать на следующий последний узел
    container.remove(iterator);
    currentSize = container.size();
    if (currentSize != (size - 10))
    {
        return false;
    }

    delete iterator;
    container.clear();
    return true;
}

bool TestAsContainer::traverse()
{
    const char *currentFunction = "    TestAsContainer::traverse()";

    Container &container = this->container();

    // Проход по пустому контейнеру
    // если iterator->hasNext() == false, то и идти не будет

    // Заполняем контейнер
    fillContainer();

    // Подготовка данных для сравнения
    // первый слой
    TestData testDataFirstLevel[5];
    for (size_t i = 0; i < 5; ++i)
    {
        testDataFirstLevel[i].i = 100 * (i + 1);
        testDataFirstLevel[i].c[0] = 10 * (i + 1);
        testDataFirstLevel[i].c[1] = 5 * (i + 1);
    }

    // Второй слой
    TestData testDataSecondLevel[15];
    for (size_t i = 0; i < 15; ++i)
    {
        testDataSecondLevel[i].i = 1000 * (i + 1);
        testDataSecondLevel[i].c[0] = 100 * (i + 1);
        testDataSecondLevel[i].c[1] = 10 * (i + 1);
    }

    TestData testData[20];
    for (size_t i = 0; i < 20;) // работает как while() с автовыходом по условию
    {
        for (size_t j = 0; j < 5; ++j)
        {
            for (size_t k = 0; k < 3; ++k)
            {
                testData[i++] = testDataSecondLevel[(j * 3) + k];
            }
            testData[i++] = testDataFirstLevel[j];
        }
    }

    // Проход по заполненному контейнеру и проверка данными
    Container::Iterator *iterator = container.newIterator();
    for (size_t i = 0; (i < 20) && iterator->hasNext(); ++i, iterator->goToNext())
    {
        size_t dataSize = 0;
        void *dataValue = iterator->getElement(dataSize);
        if ((dataValue == nullptr) || (dataSize != sizeof(TestData)))
        {
            delete iterator;
            std::cout << currentFunction << ": Iterator doesn't have any data"
                      << std::endl;
            return false;
        }
        TestData &iteratorData = *static_cast<TestData *>(dataValue);
        TestData &currentData = testData[i];
        if (iteratorData != currentData)
        {
            delete iterator;
            std::cout << currentFunction << ": Test data doesn't equal iterator's data"
                      << std::endl;
            return false;
        }
    }

    delete iterator;
    container.clear();
    return true;
}
