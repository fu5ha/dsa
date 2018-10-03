#include <stdio.h>
#include "error.h"
#include "util.h"
#include "heap.h"

int main(int argc, char const *argv[])
{
    Heap heap = Initialize(15);
    Element arr[15] = {Elem(1), Elem(2), Elem(5), Elem(10), Elem(25), Elem(19), Elem(3), Elem(45), Elem(32), Elem(15), Elem(67), Elem(22), Elem(4), Elem(33), Elem(40)};

    Error e = BuildHeap(&heap, arr, 15, 1);
    if (!IsOK(&e)) {
        printf("Error: %s\n", e.msg);
    }

    e = Insert(&heap, 99, 1);
    if (!IsOK(&e)) {
        printf("Error: %s\n", e.msg);
    }

    e = DeleteMax(&heap, 1);
    if (!IsOK(&e)) {
        printf("Error: %s\n", e.msg);
    }

    return 0;
}
