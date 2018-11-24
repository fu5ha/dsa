#ifndef GRAPH_H
#define GRAPH_H

#include "error.h"
#include "node.h"
#include "heap.h"

typedef struct {
    int p;
    int dist;
    AdjNode* adj;
} Vertex;

typedef struct {
    Heap V;
    Vertex* vertices;
    int initialized;
    int n_v;
    int n_e;
} Graph;

Graph InitializeGraph(int n_v, int n_e);
Error BuildGraph(Graph* g, Vertex* vertices);
void FreeGraph(Graph* g);
Error printGraph(Graph* g);
Error ShortestPath(Graph* g, int s, int t, int f);

void AddEdgeAdj(AdjNode* node, int neighbor, int weight);
void AddEdge(Vertex* V, int node, int neighbor, int weight);

#endif