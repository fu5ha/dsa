#ifndef GRAPH_H
#define GRAPH_H

#include "error.h"
#include "node.h"
#include "heap.h"

typedef struct {
    Heap* S;
    Heap* V;
    Element* vertices;
    int n_v;
    int n_e;
} Graph;

Graph InitializeGraph(int n_v, int n_e);
Error BuildGraph(Graph* g, Element* vertices);
void FreeGraph(Graph* g);
Error printGraph(Graph* g);

void AddEdgeAdj(AdjNode* node, int neighbor, int weight);
void AddEdge(Element* G, int node, int neighbor, int weight);

#endif