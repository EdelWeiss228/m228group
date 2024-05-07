#include "derevo.h"
#include "Mem.h"
using namespace std;


int main(){
    Mem mm(999);
    cout<<mm.size();
    Tree *derevo = new Tree(mm);
    return 0;
}