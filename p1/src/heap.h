#ifndef HEAP_H
#define HEAP_H

#include "error.h"

typedef struct {
    int key;
} Element;

Element Elem(int key);

typedef struct {
    int exists;
    int idx;
} MaybeElement;

typedef struct {
    int capacity;
    int size;
    Element* H;
} Heap;

Heap Initialize(int n);

void printHeap(Heap* heap);
void printRaw(Heap* heap); 
void printPrettyTree(Heap* heap);

Error BuildHeap(Heap* heap, Element* A, int n, int flag);
Error Insert(Heap* heap, int k, int flag);
Error DeleteMax(Heap* heap, int flag);
Error IncreaseKey(Heap* heap, int index, int value, int flag);
Error Heapify(Heap* heap, int index);

MaybeElement LeftChild(Heap* heap, int index);
MaybeElement RightChild(Heap* heap, int index);
int isSome(MaybeElement* elem);

#endif