#ifndef UTIL_H
#define UTIL_H

#include "error.h"
#include "heap.h"
#include "graph.h"

// Helper struct to contain input parser state
typedef struct {
    char c;
    int s;
    int t;
    int f;
    int shouldPrint;
} State;

// Parses a command from stdin
int nextCommand(State* state, char* msg);

// Reads and parses HEAPInput file
Error ReadFile(Vertex** vertices, int* n_v, int* n_e);

#endif