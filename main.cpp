#include "derevo.h"
#include "Mem.h"
using namespace std;


int main(){
    Mem mm(999);
    cout<<mm.size();
    tree *derevo = new tree(mm);
    return 0;
}