#ifndef ERROR_H
#define ERROR_H

// Helper class to represent some kind of error that may occur as well as
// a message to go along with it.
typedef struct {
    int kind;
    char* msg;
} Error;

// Checks if the error contains an error or is OK
int IsOK(Error* e);

// Helper functions to create Errors of different kinds

Error Ok();
Error OutOfBounds(int i, int max);
Error OverCapacity(int i, int max);
Error KeyGreaterThanCurrent(int current, int key);
Error CannotOpenFile();
Error HeapUninitialized();
Error GraphUninitialized();
Error TooFewElements();
Error DeleteZeroElements();
Error InputFileEmpty();

#endif