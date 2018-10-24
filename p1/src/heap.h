#ifndef HEAP_H
#define HEAP_H

#include "error.h"

// Type of an element inside the Heap
typedef struct {
    int key;
} Element;

// Utility function to create an element with the given key
Element Elem(int key);

// Struct that represents a heap
typedef struct {
    int capacity;
    int size;
    Element* H;
} Heap;

// Iniitlaizes a new heap with capacity n
Heap Initialize(int n);

// Prints the capacity, size, and key values of the elements in the Heap
Error printHeap(Heap* heap);
// Helper function that prints the raw key values of the elements in the
// Heap in the order they occur in the inner array.
void printRaw(Heap* heap); 
// Helper function that prints the key values of the elements in the 
// Heap in a tree view form, used only for debugging purposes.
void printPrettyTree(Heap* heap);

// Builds a max heap out of a raw array of size n
Error BuildHeap(Heap* heap, Element* A, int n, int flag);

// Inserts an element with key k into the max heap
Error Insert(Heap* heap, int k, int flag);

// Deletes the max element from the heap
Error DeleteMax(Heap* heap, int flag);

// Increases key at the index to key of value
Error IncreaseKey(Heap* heap, int index, int value, int flag);

// Recursively create a max heap assuming the children of the element at index
// are already max heaps but the element at index may violate the max heap rule
Error Heapify(Heap* heap, int index);

// Helper type which contains the potential index of an element
// and whether that element exists or not.
typedef struct {
    int exists;
    int idx;
} MaybeElement;

// Utility function to get the left child of an element. Since it could exist or
// not exist, we return a MaybeElement which either contains the element or
// None if it does not exist.
MaybeElement LeftChild(Heap* heap, int index);

// Utility function to get the right child of an element. Since it could exist or
// not exist, we return a MaybeElement which either contains the element or
// None if it does not exist.
MaybeElement RightChild(Heap* heap, int index);

// Checks whether a MaybeElement contains an element or is None.
int isSome(MaybeElement* elem);

#endif