#include "graph.h"
#include "heap.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

Graph InitializeGraph(int n_v, int n_e) {
    Heap V = InitializeHeap(n_v);
    return (Graph){V, NULL, 0, n_v, n_e};
}

Error BuildGraph(Graph* g, Vertex* vertices) {
    g->vertices = vertices;
    g->intialized = 1;
    Element* A = malloc(sizeof(Element) * g->n_v);
    for (int i = 0; i < g->n_v; i++) {
        A[i] = Elem(INT_MAX, i);
    }
    Error e = BuildHeap(&g->V, A, g->n_v, 0);
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

void AddEdge(Vertex* V, int node, int neighbor, int weight) {
    // printf("Adding edge: %d -> %d (%d)\n", node, neighbor, weight);
    if (V[node].adj == NULL) {
        V[node].adj = malloc(sizeof(AdjNode));
        V[node].adj->neighbor = neighbor;
        V[node].adj->weight = weight;
        V[node].adj->next = NULL;
    } else {
        AddEdgeAdj(V[node].adj, neighbor, weight);
    }
}

Error printGraph(Graph* g) {
    if (!g->initialized) {
        return GraphUninitialized();
    }

    printf("%d %d\n", g->n_v, g->n_e);
    for (int i=0; i < g->n_v; i++) {
        printf("%d: ", i + 1);
        AdjNode *n = g->vertices[i].adj;
        while (n != NULL) {
            printf("(%d, %d); ", n->neighbor + 1, n->weight);
            n = n->next;
        }
        printf("\n");
    }

    return Ok();
    
}

void FreeGraph(Graph* g) {
    free(g->V.H);

    for (int i = 0; i < g->n_v; i++) {
        AdjNode* n = g->vertices[i].adj;
        while (n != NULL) {
            AdjNode* temp = n;
            n = n->next;
            free(temp);
        }
    }

    free(g->vertices);
}

Error initializeSingleSource(Graph* g, int s) {
    g->V.size = g->n_v;
    for (int i = 0; i < g->n_v; i++) {
        g->V.H[i].v_idx = i;
        g->V.H[i].key = INT_MAX;
        g->vertices[i].dist = INT_MAX;
    }
    g->vertices[s].dist = 0;
    Error e = DecreaseKey(&g->V, s, 0, 0);
    if (!IsOK(&e)) {
        return e;
    }
    return Ok();
}

Error Relax(Graph* g, int u, int v, int w) {
    int ud = g->vertices[u].dist;
    int vd = g->vertices[v].dist;
    if (vd > ud + w) {
        MaybeElement el = FindVertex(&g->V, v);
        if (isSome(&el)) {
            g->vertices[v].dist = ud + w;
            g->vertices[v].p = u;
            Error e = DecreaseKey(&g->V, el.idx, ud + w, 0);
            if (!IsOK(&e)) {
                return e;
            }
        }
    }
    return Ok();
}

Error ShortestPath(Graph* g, int s, int t, int f) {
    if (!g->initialized) {
        return GraphUninitialized();
    }

    if (s < 0 || s >= g->n_v || t < 0 || t >= g->n_v) {
        return NodesOutOfBounds();
    }

    initializeSingleSource(g, s);

    Error e = Ok();
    while (g->V.size > 0) {
        Element u = g->V.H[0];
        e = DeleteMin(&g->V, 0);
        if (!IsOK(&e)) {
            return e;
        }
        for (AdjNode* adj = g->vertices[u.v_idx].adj; adj != NULL; adj = adj->next) {
            e = Relax(g, u.v_idx, adj->neighbor, adj->weight);
            if (!IsOK(&e)) {
                return e;
            }
        }
    }

    if (g->vertices[t].dist == INT_MAX) {
        return CannotReach(s, t);
    }

    if (f == 0) {
        printf("LENGTH: %d\n", g->vertices[t].dist);
    }

    if (f == 1) {
        int* stack = malloc(sizeof(int) * g->n_v);
        int idx = -1;
        for (int n = t; n != s; n = g->vertices[n].p) {
            // printf("%d, ", n + 1);
            stack[++idx] = n + 1;
        }
        stack[++idx] = s + 1;

        printf("PATH: ");
        for (int i = idx; i > 0; i--) {
            printf("%d, ", stack[i]);
        }
        printf("%d\n", stack[0]);
        free(stack);
    }

    return Ok();
}