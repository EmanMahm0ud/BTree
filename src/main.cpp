#include <iostream>
#include "BTree.h"

using namespace std;

int main()
{
    BTree<int,5> t1;

    t1.Insert(8);
    t1.Insert(14);
    t1.Insert(2);
    t1.Insert(15);
    t1.Insert(3);
    t1.Insert(1);
    t1.Insert(16);
    t1.Insert(6);
    t1.Insert(5);
    t1.Insert(27);
    t1.Insert(37);
    t1.Insert(18);
    t1.Insert(25);
    t1.Insert(7);
    t1.Insert(13);
    t1.Insert(20);
    t1.Insert(22);
    t1.Insert(23);
    t1.Insert(24);

    t1.Print();

    /*
    16
      3,8
        1,2
        5,6,7
        13,14,15
      22,25
        18,20
        23,24
        27,37
    */

    return 0;
}
