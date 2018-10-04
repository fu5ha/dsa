#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "util.h"
#include "heap.h"

int main(int argc, char const *argv[])
{
    Heap heap;
    Element* arr = malloc(sizeof(Element));
    int arr_l = 1;

    State state = (State){0, 0, 0, 0};
    char* msgbuf = malloc(50);

    Error e = Ok();
    while (nextCommand(&state, msgbuf)) {
        printf("COMMAND: %s\n", msgbuf);
        if (state.c == 'C') {
            heap = Initialize(state.n);
        }
        if (state.c == 'W') {
            printHeap(&heap);
        }
        if (state.c == 'I') {
            e = Insert(&heap, state.v, state.f);
        }
        if (state.c == 'K') {
            e = IncreaseKey(&heap, state.i, state.v, state.f);
        }
        if (state.c == 'D') {
            e = DeleteMax(&heap, state.f);
        }
        if (state.c == 'R') {
            e = ReadFile(&arr, &arr_l);
            if (IsOK(&e)) {
                e = BuildHeap(&heap, arr, arr_l, state.f);
            }
        }

        if (!IsOK(&e)) {
            printf("Error: %s\n", e.msg);
        }
    }

    free(msgbuf);
    free(arr);
    free(heap.H);

    printf("STOPPING\n");

    return 0;
}
