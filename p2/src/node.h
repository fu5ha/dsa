#ifndef NODE_H
#define NODE_H

#include "error.h"

typedef struct adjnode {
    int neighbor;
    int weight;
    struct adjnode* next;
} AdjNode;

#endif