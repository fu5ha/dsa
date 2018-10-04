#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "util.h"

int nextCommand(State* state, char* msg) {
    while (1) {
        scanf("%c", &state->c);
        char c = state->c;
        if (c == ' ' || c == '\t' || c == '\n') {
            continue;
        }
        if (c == 'S' || c == 's') {
            strcpy(msg, "S\0");
            return 0;
        }
        if (c == 'C' || c == 'c') {
            scanf("%d", &state->n);
            state->c = 'C';
            sprintf(msg, "C %d", state->n);
            return 1;
        }
        if (c == 'W' || c == 'w') {
            strcpy(msg, "W\0");
            return 1;
        }
        if (c == 'I' || c == 'i') {
            scanf("%d", &state->v);
            scanf("%d", &state->f);
            state->c = 'I';
            sprintf(msg, "I %d %d", state->v, state->f);
            return 1;
        }
        if (c == 'D' || c == 'd') {
            scanf("%d", &state->f);
            state->c = 'D';
            sprintf(msg, "D %d", state->f);
            return 1;
        }
        if (c == 'K' || c == 'k') {
            scanf("%d", &state->i);
            scanf("%d", &state->v);
            scanf("%d", &state->f);
            state->c = 'K';
            sprintf(msg, "K %d %d %d", state->i, state->v, state->f);
            return 1;
        }
        if (c == 'R' || c == 'r') {
            scanf("%d", &state->f);
            state->c = 'R';
            sprintf(msg, "R %d", state->f);
            return 1;
        }
        strcpy(msg, "Invalid Command\0");
        return 1;
    }
}

Error ReadFile(Element** arr, int* arr_l) {
    FILE* fptr = fopen("HEAPinput.txt", "r");

    if (fptr == NULL) {
        return CannotOpenFile();
    }

    fscanf(fptr, "%d\n", arr_l);

    (*arr) = malloc(sizeof(Element) * (*arr_l));

    int k = 0;
    for (int i = 0; i < (*arr_l); i++) {
        fscanf(fptr, "%d\n", &k);
        (*arr)[i] = (Element){k};
    }

    fclose(fptr);

    return Ok();
}