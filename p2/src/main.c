#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "util.h"
#include "graph.h"

int main(int argc, char const *argv[])
{
    // Create Heap and Element arrays for use later
    Graph graph;
    Vertex* vertices = malloc(sizeof(Vertex));
    int n_v = 1;
    int n_e = 1;

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

        if (state.c == 'W') {
            e = printGraph(&graph);
        }

        if (state.c == 'R') {
            e = ReadFile(&vertices, &n_v, &n_e);
            if (IsOK(&e)) {
                graph = InitializeGraph(n_v, n_e);
                e = BuildGraph(&graph, vertices);
            }
        }

        if (state.c == 'P') {
            e = ShortestPath(&graph, state.s - 1, state.t - 1, state.f);
        }

        if (!IsOK(&e)) {
            printf("Error: %s\n", e.msg);
        }
    }

    // Free relevant memory
    free(msgbuf);
    FreeGraph(&graph);

    printf("STOPPING\n");

    return 0;
}
