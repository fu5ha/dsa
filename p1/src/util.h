#ifndef UTIL_H
#define UTIL_H

#include "error.h"
#include "heap.h"

// Helper struct to contain input parser state
typedef struct {
    char c;
    int i;
    int v;
    int n;
    int f;
    int shouldPrint;
} State;

// Parses a command from stdin
int nextCommand(State* state, char* msg);

// Reads and parses HEAPInput file
Error ReadFile(Element** arr, int* arr_l);

#endif