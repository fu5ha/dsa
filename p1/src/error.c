#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "error.h"

const Error OK = (Error){0, NULL};
const int OUT_OF_BOUNDS = 1;

int IsOK(Error *e) {
    return e->kind == 0;
}

Error Ok() {
    return OK;
}

Error OutOfBounds(int i, int max) {
    char* msg = (char*)malloc(20 + sizeof(char) * 47);
    sprintf(msg, "Out of bounds index: got index %d, max expected %d", i, max);
    return (Error){1, msg};
}

Error OverCapacity(int i, int max) {
    char* msg = (char*)malloc(20 + sizeof(char) * 41);
    sprintf(msg, "Not enough capacity: max %d, tried to use %d", max, i);
    return (Error){1, msg};
}

Error KeyLessThanCurrent(int current, int key) {
    char* msg = (char*)malloc(20 + sizeof(char) * 51);
    sprintf(msg, "Key in IncreaseKey less than current: current %d, new %d", current, key);
    return (Error){1, msg};
}
