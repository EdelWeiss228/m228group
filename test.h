#include <iostream>
#include <cassert>
#include "derevo.h"

void test_iterator(Tree& tree) {
    Tree::Iterator* iter = tree.newIterator();
    assert(iter != nullptr); // Проверка на nullptr

    int element1 = 42;
    int element2 = 84;
    size_t size1 = sizeof(element1);
    size_t size2 = sizeof(element2);

    // Вставляем два элемента
    assert(tree.insert(iter, 0, &element1, size1) == 0);
    assert(tree.insert(iter, 1, &element2, size2) == 0);
    std::cout << "Два элемента успешно вставлены" << std::endl;

    // Перемещаемся к следующему элементу
    iter->goToChild(0); // Убедитесь, что начинаем с первого ребенка
    iter->goToNext();   // Теперь перемещаемся к следующему элементу

    size_t size;
    int* current_element = static_cast<int*>(iter->getElement(size));
    assert(current_element != nullptr); // Проверка на nullptr

    // Проверяем второй элемент
    assert(*current_element == element2);
    std::cout << "Текущий элемент: " << *current_element << std::endl;
    std::cout << "4 vypolnen" << std::endl;
}



void test_insert_and_size(Tree& tree) {
    Tree::Iterator* iter = tree.newIterator();
    assert(iter != nullptr); // Проверка на nullptr

    int element = 42;
    size_t size = sizeof(element);

    // Вставляем элемент
    int result = tree.insert(iter, 0, &element, size);
    assert(result == 0); // Успешная вставка
    std::cout << "Вставка успешна, результат: " << result << std::endl;

    // Проверяем размер дерева
    assert(tree.size() == 1);
    std::cout << "Размер дерева после вставки: " << tree.size() << std::endl;
    std::cout << "1 vypolnen" << std::endl;
    delete iter; // Освобождаем память
}



void test_remove(Tree& tree) {
    // Создаем итератор и проверяем, что он не равен nullptr
    Tree::Iterator* iter = tree.newIterator();
    assert(iter != nullptr); 

    int element = 42;
    size_t size = sizeof(element);

    // Вставляем элемент
    int insert_result = tree.insert(nullptr, 0, &element, size); // Используем nullptr, чтобы вставить в корень
    assert(insert_result == 0); 
    std::cout << "Вставка для удаления успешна" << std::endl;

    // Обновляем итератор
    delete iter; // Удаляем старый итератор
    iter = tree.newIterator(); // Создаем новый итератор

    // Убедитесь, что итератор указывает на элемент
    assert(iter->returnNode() != nullptr);

    // Удаляем элемент
    bool removed = tree.remove(iter, true);
    assert(removed == true);
    std::cout << "Удаление успешное: " << removed << std::endl;

    // Проверяем, что дерево пустое
    assert(tree.size() == 0);
    assert(tree.empty() == true);
    std::cout << "Размер дерева после удаления: " << tree.size() << std::endl;
    std::cout << "2 vypolnen" << std::endl;
    delete iter; // Освобождаем память
}



void test_find(Tree& tree) {
    Tree::Iterator* iter = tree.newIterator();
    assert(iter != nullptr); // Проверка на nullptr

    int element = 42;
    size_t size = sizeof(element);

    // Вставляем элемент
    tree.insert(iter, 0, &element, size);
    std::cout << "Вставка для поиска успешна" << std::endl;

    // Ищем элемент
    Tree::Iterator* found_iter = tree.find(&element, size);
    assert(found_iter != nullptr);
    std::cout << "Элемент найден" << std::endl;

    size_t found_size;
    int* found_element = static_cast<int*>(found_iter->getElement(found_size));
    assert(found_element != nullptr); // Проверка на nullptr

    // Проверяем, что элемент найден и совпадает
    assert(*found_element == element);
    std::cout << "Элемент совпадает: " << *found_element << std::endl;
    std::cout << "3 vypolnen" << std::endl;
    }

