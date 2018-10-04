#ifndef UTIL_H
#define UTIL_H

#include "error.h"
#include "heap.h"

typedef struct {
    char c;
    int i;
    int v;
    int n;
    int f;
} State;

int nextCommand(State* state, char* msg);
Error ReadFile(Element** arr, int* arr_l);

#endif