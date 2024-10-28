#include "derevo.h"
#include "Mem.h"
#include "List_1.h"
#include "test.h"
using namespace std;


int main(){
    Mem mem (10000);
    TestTree testTree (mem);
    testTree.testTree1andTree2(5,10);
    testTree.testNewIterator();
    testTree.testGoToChild();
    testTree.testGoToParent();
    testTree.testHasNext();
    testTree.testEquals();
    testTree.testSize();
    testTree.testEmpty();
    testTree.testFind();
    testTree.testInsert();
    testTree.testRemove();
    testTree.testClear();
    testTree.testDifferentTypes();
    testTree.testPerformance();
}