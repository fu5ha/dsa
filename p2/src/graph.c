#include "graph.h"
#include "heap.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

// Initializes a graph with empty edge adjacenty list and empty heap with capacity
// equal to number of vertices
Graph InitializeGraph(int n_v, int n_e) {
    Heap V = InitializeHeap(n_v);
    return (Graph){V, NULL, 0, n_v, n_e};
}

// Builds the graph and the inner heap based on given edge adjacency list.
Error BuildGraph(Graph* g, Vertex* vertices) {
    // Set edge adjacency list to given
    g->vertices = vertices;
    // Set graph as initialized
    g->initialized = 1;
    // Allocate and create initial element array for building the heap
    Element* A = malloc(sizeof(Element) * g->n_v);
    for (int i = 0; i < g->n_v; i++) {
        A[i] = Elem(INT_MAX, i);
    }
    // Build the heap
    Error e = BuildHeap(&g->V, A, g->n_v, 0);
    if (!IsOK(&e)) {
        return e;
    }
    return Ok();
}

// Inner helper function which recursively finds the last edge in an
// edge adjacency list then apends a new one
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

// Adds an edge to the specified node in an edge adjacency list
void AddEdge(Vertex* V, int node, int neighbor, int weight) {
    // If the node's edge adjacenty list doesn't have any nodes yet then just add it
    if (V[node].adj == NULL) {
        V[node].adj = malloc(sizeof(AdjNode));
        V[node].adj->neighbor = neighbor;
        V[node].adj->weight = weight;
        V[node].adj->next = NULL;
    } else {
        // If it does then use our recursive function to add it.
        AddEdgeAdj(V[node].adj, neighbor, weight);
    }
}

// Prints the graph's edge adjacenty list
Error printGraph(Graph* g) {
    if (g == NULL || !g->initialized) {
        return GraphUninitialized();
    }

    printf("%d %d\n", g->n_v, g->n_e);
    for (int i=0; i < g->n_v; i++) {
        printf("%d: ", i + 1);
        AdjNode *n = g->vertices[i].adj;
        while (n != NULL) {
            printf("(%d, %d)", n->neighbor + 1, n->weight);
            if (n->next != NULL) {
                printf("; ");
            }
            n = n->next;
        }
        printf("\n");
    }

    return Ok();
    
}

// Frees allocated memory related to the graph
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

// Helper function that resets the internal heap and
// vertex array to be ready for a new run of Dijkstra's algorithm.
Error initializeSingleSource(Graph* g, int s) {
    // Reset heap size
    g->V.size = g->n_v;
    // Set heap keys and associate them with vertex indices
    for (int i = 0; i < g->n_v; i++) {
        g->V.H[i].v_idx = i;
        g->V.H[i].key = INT_MAX;
        g->vertices[i].dist = INT_MAX;
    }
    g->vertices[s].dist = 0;
    // Decrease key on the source vertex to 0
    Error e = DecreaseKey(&g->V, s, 0, 0);
    if (!IsOK(&e)) {
        return e;
    }
    return Ok();
}

// Relaxes v with respect to parent u and the edge weight w
Error Relax(Graph* g, int u, int v, int w) {
    int ud = g->vertices[u].dist;
    int vd = g->vertices[v].dist;

    // If the current known distance is larger than the sum of parent and weight, then
    if (vd > ud + w) {
        MaybeElement el = FindVertex(&g->V, v);
        if (isSome(&el)) {
            // Make current distance equal to parent plus weight and set parent node to u
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

// Runs Dijkstra's algorithm on the graph g to find shortest path from s to t.
Error ShortestPath(Graph* g, int s, int t, int f) {
    // Make sure graph is initialized
    if (g == NULL || !g->initialized) {
        return GraphUninitialized();
    }

    // Make sure start and end nodes are within bounds of the graph
    if (s < 0 || s >= g->n_v || t < 0 || t >= g->n_v) {
        return NodesOutOfBounds();
    }

    // Perform initialization
    initializeSingleSource(g, s);

    Error e = Ok();
    // While there are still nodes left in the min heap
    while (g->V.size > 0) {
        // Extract the minimum
        Element u = g->V.H[0];
        e = DeleteMin(&g->V, 0);
        if (!IsOK(&e)) {
            return e;
        }
        // Relax all nodes that are on its edge adjacency list
        for (AdjNode* adj = g->vertices[u.v_idx].adj; adj != NULL; adj = adj->next) {
            e = Relax(g, u.v_idx, adj->neighbor, adj->weight);
            if (!IsOK(&e)) {
                return e;
            }
        }
    }

    // If the destination still has INT_MAX, then it was unreachable
    if (g->vertices[t].dist == INT_MAX) {
        return CannotReach(s, t);
    }

    // If flag is 0, print out dist at t which is the length.
    if (f == 0) {
        printf("LENGTH: %d\n", g->vertices[t].dist);
    }

    // If flag is 1, use a basic stack to print out the path
    if (f == 1) {
        int* stack = malloc(sizeof(int) * g->n_v);
        int idx = -1;
        // Start at t, follow its parents up the graph and pushing
        // each onto a stack.
        for (int n = t; n != s; n = g->vertices[n].p) {
            // printf("%d, ", n + 1);
            stack[++idx] = n + 1;
        }
        stack[++idx] = s + 1;

        printf("PATH: ");
        // Pop off the stack, printing each node as we do so
        for (int i = idx; i > 0; i--) {
            printf("%d, ", stack[i]);
        }
        printf("%d\n", stack[0]);
        free(stack);
    }

    return Ok();
}