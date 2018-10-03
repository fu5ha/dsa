#ifndef ERROR_H
#define ERROR_H

typedef struct {
    int kind;
    char* msg;
} Error;

int IsOK(Error* e);
Error Ok();
Error OutOfBounds(int i, int max);
Error OverCapacity(int i, int max);
Error KeyLessThanCurrent(int current, int key);

#endif