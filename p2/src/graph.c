#include "graph.h"
#include <stdlib.h>
#include <stdio.h>

Graph InitializeGraph(int n_v, int n_e) {
    Heap S = InitializeHeap(n_v);
    Heap V = InitializeHeap(n_v);
    return (Graph){&S, &V, NULL, n_v, n_e};
}

Error BuildGraph(Graph* g, Element* vertices) {
    g->vertices = vertices;
    Error e = BuildHeap(g->V, vertices, g->n_v, 0);
    if (!IsOK(&e)) {
        return e;
    }
    return Ok();
}

void AddEdgeAdj(AdjNode* node, int neighbor, int weight) {
    if (node->next == NULL) {
        node->next = malloc(sizeof(AdjNode));
        node->next->neighbor = neighbor;
        node->next->weight = weight;
        node->next->next = NULL;
    } else {
        AddEdgeAdj(node->next, neighbor, weight);
    }
}

void AddEdge(Element* G, int node, int neighbor, int weight) {
    printf("Adding edge: %d -> %d (%d)\n", node, neighbor, weight);
    if (G[node].adj == NULL) {
        G[node].adj = malloc(sizeof(AdjNode));
        G[node].adj->neighbor = neighbor;
        G[node].adj->weight = weight;
        G[node].adj->next = NULL;
    } else {
        AddEdgeAdj(G[node].adj, neighbor, weight);
    }
}

Error printGraph(Graph* g) {
    if (g->vertices == NULL) {
        return GraphUninitialized();
    }

    printf("%d %d\n", g->n_v, g->n_e);
    for (int i=0; i < g->n_v; i++) {
        printf("%d: ", i + 1);
        AdjNode *n = g->vertices[i].adj;
        while (n != NULL) {
            printf("(%d, %d); ", n->neighbor, n->weight);
            n = n->next;
        }
        printf("\n");
    }

    return Ok();
    
}

void FreeGraph(Graph* g) {
    free(g->S->H);
    free(g->V->H);

    for (int i = 0; i < g->n_v; i++) {
        if (g->vertices[i].adj != NULL) {
            AdjNode* n = g->vertices[i].adj;
            while (n != NULL) {
                AdjNode* temp = n;
                n = n->next;
                free(temp);
            }
        }
    }

    free(g->vertices);
}