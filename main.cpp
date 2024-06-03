#include "derevo.h"
#include "Mem.h"
#include "List_1.h"
#pragma once
using namespace std;


int main(){
    Mem mm(999);
    cout<<mm.size();
    Tree *derevo = new Tree(mm);
    List *list = new List(mm);
    return 0;
}