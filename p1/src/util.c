#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "util.h"

// Parses a command from stdin
int nextCommand(State* state, char* msg) {
    while (1) {
        scanf("%c", &state->c);
        char c = state->c;
        if (c == ' ' || c == '\t' || c == '\n') {
            continue;
        }
        if (c == 'S' || c == 's') {
            state->shouldPrint = 1;
            strcpy(msg, "S\0");
            printf("COMMAND: S\n");
            return 0;
        }
        if (c == 'C' || c == 'c') {
            int matches = scanf("%d", &state->n);
            if (matches != 0) {
                state->shouldPrint = 1;
                state->c = 'C';
                sprintf(msg, "C %d", state->n);
                return 1;
            }
        }
        if (c == 'W' || c == 'w') {
            strcpy(msg, "W\0");
            return 1;
        }
        if (c == 'I' || c == 'i') {
            int matches = scanf("%d", &state->v);
            matches += scanf("%d", &state->f);
            if (matches == 2) {
                state->shouldPrint = 1;
                state->c = 'I';
                sprintf(msg, "I %d %d", state->v, state->f);
                return 1;
            }
        }
        if (c == 'D' || c == 'd') {
            int matches = scanf("%d", &state->f);
            if (matches != 0) {
                state->shouldPrint = 1;
                state->c = 'D';
                sprintf(msg, "D %d", state->f);
                return 1;
            }
        }
        if (c == 'K' || c == 'k') {
            int matches = scanf("%d", &state->i);
            matches += scanf("%d", &state->v);
            matches += scanf("%d", &state->f);
            if (matches == 3) {
                state->shouldPrint = 1;
                state->c = 'K';
                sprintf(msg, "K %d %d %d", state->i, state->v, state->f);
                return 1;
            }
        }
        if (c == 'R' || c == 'r') {
            int matches = scanf("%d", &state->f);
            if (matches != 0) {
                state->shouldPrint = 1;
                state->c = 'R';
                sprintf(msg, "R %d", state->f);
                return 1;
            }
        }

        state->c = (char)0;
        strcpy(msg, "INVALID COMMAND!\0");
        return 1;
    }
}

// Reads and parses HEAPInput file
Error ReadFile(Element** arr, int* arr_l) {
    // Open the file for reading
    FILE* fptr = fopen("HEAPinput.txt", "r");

    // Check if file open was successful
    if (fptr == NULL) {
        return CannotOpenFile();
    }

    // Scan first line for number of elements
    int matches = fscanf(fptr, "%d\n", arr_l);
    // If we didn't find a number, then return an error
    if (matches == 0) {
        return InputFileEmpty();
    }

    // Allocate an array of the proper size based on first line
    (*arr) = malloc(sizeof(Element) * (*arr_l));

    // Scan the number of lines for each number
    int k = 0;
    for (int i = 0; i < (*arr_l); i++) {
        int matches = fscanf(fptr, "%d\n", &k);
        // If we find a match, add it to the array; else return an error
        if (matches > 0) {
            (*arr)[i] = (Element){k};
        } else {
            return TooFewElements();
        }
    }

    // Close the file
    fclose(fptr);

    return Ok();
}