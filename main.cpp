#include "derevo.h"
#include "Mem.h"

#include "TestAsContainer.h"
#include "TestAsTree.h"

#include <iostream>

int main()
{
    // Mem mem{std::numeric_limits<size_t>::max()};
    // Tree tree{mem};
    // Tree::Iterator *iterator = tree.newIterator();
    // TestData data;
    // tree.insert(iterator, 0, &data, sizeof(data));
    // if (iterator->goToChild(0))
    // {
    //     size_t dataSize = 0;
    //     TestData *data = reinterpret_cast<TestData *>(iterator->getElement(dataSize));

    //     std::cout << "data->i = " << data->i
    //               << "; data->c = { " << data->c[0] << ", " << data->c[1] << " }" << std::endl;

    //     iterator->goToParent();
    // }
    // delete iterator;

    TestAsTree treeTest;
    TestAsContainer containerTest;

    std::cout << "Run container test" << std::endl;
    containerTest.run();
    std::cout << "Container test result: " << std::boolalpha << containerTest.result() << std::endl;

    std::cout << "Run tree test" << std::endl;
    treeTest.run();
    std::cout << "Tree test result: " << std::boolalpha << treeTest.result() << std::endl;

    return 0;
}
