#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "util.h"
#include "heap.h"

int main(int argc, char const *argv[])
{
    // Create Heap and Element arrays for use later
    Heap heap;
    Element* arr = malloc(sizeof(Element));
    int arr_l = 1;

    // Create input state and message buffer for input handler
    State state = (State){0, 0, 0, 0, 01};
    char* msgbuf = malloc(50);

    Error e = Ok();
    // While the command is not to stop, parse input
    while (nextCommand(&state, msgbuf)) {
        if (state.shouldPrint) {
            printf("COMMAND: %s\n", msgbuf);
            if (state.c == (char)0) {
                state.shouldPrint = 0;
            }
        }
        if (state.c == 'C') {
            heap = Initialize(state.n);
        }
        if (state.c == 'W') {
            e = printHeap(&heap);
        }
        if (state.c == 'I') {
            e = Insert(&heap, state.v, state.f);
        }
        if (state.c == 'K') {
            e = IncreaseKey(&heap, state.i - 1, state.v, state.f);
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

    // Free relevant memory
    free(msgbuf);
    free(arr);
    free(heap.H);

    printf("STOPPING\n");

    return 0;
}
