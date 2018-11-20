#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#include "heap.h"

// #define PRINT_TREE

// Utility function to create an element with the given key
Element Elem(int key) {
    return (Element){key, INT_MAX, NULL};
}

// Utility function to get the parent element of an element with given index
int Parent(int index) {
    return floor(((float)index - 1.0) / 2.0);
}

// Utility function to get the left child of an element. Since it could exist or
// not exist, we return a MaybeElement which either contains the element or
// None if it does not exist.
MaybeElement LeftChild(Heap* heap, int index) {
    int newIdx = index * 2 + 1;
    int exists;
    if (newIdx < heap->size) {
        exists = 1;
    } else {
        exists = 0;
    }
    return (MaybeElement){exists, newIdx};
}

// Utility function to get the right child of an element. Since it could exist or
// not exist, we return a MaybeElement which either contains the element or
// None if it does not exist.
MaybeElement RightChild(Heap* heap, int index) {
    int newIdx = index * 2 + 2;
    int exists;
    if (newIdx < heap->size) {
        exists = 1;
    } else {
        exists = 0;
    }
    return (MaybeElement){exists, newIdx};
}

// Checks whether a MaybeElement contains an element or is None.
int isSome(MaybeElement* elem) {
    return elem->exists;
}

// Iniitlaizes a new heap with capacity n
Heap InitializeHeap(int n) {
    Element* H = malloc(sizeof(Element) * n);
    Heap heap = {
        .capacity = n,
        .size = 0,
        .H = H,
    };
    return heap;
}

// Utility function that gets the key value of the element at an index
int keyAt(Heap* heap, int index) {
    return heap->H[index].key;
}

// Helper function that prints the raw key values of the elements in the
// Heap in the order they occur in the inner array.
void printRaw(Heap* heap) {
    for (int i = 0; i < heap->size; i++) {
        printf("%d\n", heap->H[i].key);
    }
}

// Helper function that prints the key values of the elements in the 
// Heap in a tree view form, used only for debugging purposes.
void printPrettyTree(Heap* heap) {
    int maxDepth = floor(log2f((float)heap->size));
    int n = 0;
    for (int depth = 0; depth <= maxDepth; depth++) {
        int nNodes = floor(powf(2.0, (float)depth));
        int height = maxDepth - depth;
        int nSpaces = floor(powf(2.0, (float)height)) + 1;
        for (int i = 0; i < nNodes; i++) {
            if (n < heap->size) {
                if (i == 0) {
                    for (int j = 0; j < nSpaces; j++) {
                        printf(" ");
                    }
                }
                printf("%d", keyAt(heap, n));
                for (int j = 0; j < nSpaces; j++) {
                    printf(" ");
                }
            }
            n++;
        }
        printf("\n");
    }
    printf("\n");
}

// Prints the capacity, size, and key values of the elements in the Heap
Error printHeap(Heap* heap) {
    if (heap->H == NULL) {
        return HeapUninitialized();
    }
    printf("The capacity is %d.\n", heap->capacity);
    printf("Size is %d.\n", heap->size);
    printRaw(heap);
    // Prints the tree view for debugging purposes if PRINT_TREE is defined
    #ifdef PRINT_TREE
    printf("Tree View: \n");
    printPrettyTree(heap);
    #endif
    return Ok();
}

// Builds a max heap out of a raw array of size n
Error BuildHeap(Heap* heap, Element* A, int n, int flag) {
    // Check if the heap is initialized
    if (heap->H == NULL) {
        return HeapUninitialized();
    }
    // Check that the capacity is sufficient
    if (heap->capacity < n) {
        return OverCapacity(n, heap->capacity);
    }
    // Since the inner representation of the heap is just an array, the
    // memory layout is the same so we can just memcpy the given array into the
    // heap's array.
    memcpy(heap->H, A, n * sizeof(Element));
    heap->size = n;

    // Index of the first element we need to call Heapify on, since it could have children
    int first = floor((float)n/2.0) - 1;
    // Iterate backwards through the tree from the first index and call Heapify on each
    for (int i = first; i >= 0; i--) {
        Error e = Heapify(heap, i);
        if (!IsOK(&e)) {
            return e;
        }
        // Print contents after each outer call to Heapify if wanted
        if (flag == 2) {
            e = printHeap(heap);
            if (!IsOK(&e)) {
                return e;
            }
        }
    }
    // Print heap contents after operation if wanted
    if (flag == 1) {
        Error e = printHeap(heap);
        if (!IsOK(&e)) {
            return e;
        }
    }

    return Ok();
}

// Helper function that swaps the elements with index first and second
Error Swap(Heap* heap, int first, int second) {
    // Check that heap is initialized
    if (heap->H == NULL) {
        return HeapUninitialized();
    }
    // Check both indices are valid inside the heap size
    if (heap->size - 1 < first) {
        return OutOfBounds(first, heap->size - 1);
    }
    if (heap->size - 1 < second) {
        return OutOfBounds(second, heap->size - 1);
    }
    // Actually swap the elements
    Element temp = heap->H[first];
    heap->H[first] = heap->H[second];
    heap->H[second] = temp;
    return Ok();
}

// Recursively create a max heap assuming the children of the element at index
// are already max heaps but the element at index may violate the max heap rule
Error Heapify(Heap* heap, int index) {
    // Check for uninitialized heap
    if (heap->H == NULL) {
        return HeapUninitialized();
    }
    // Check that index is inside valid heap size
    if (heap->size - 1 < index) {
        return OutOfBounds(index, heap->size - 1);
    }

    // Get the key value of the element at index
    int key = keyAt(heap, index);

    // Variables which will hold the min key of the children
    // and the index of that element
    int minChildKey;
    int minChildIdx;

    // Get objects which represent either an element or the absence
    // of an element given that the children may or may not exist.
    MaybeElement left = LeftChild(heap, index);
    MaybeElement right = RightChild(heap, index);

    // If the right child exists then the left child must also exist
    if (isSome(&right)) {
        // Therefore we get the keys of both children
        int leftKey = heap->H[left.idx].key;
        int rightKey = heap->H[right.idx].key;
        // And assign the min between them to the minChildKey and minChildIdx respectively
        if (leftKey < rightKey) {
            minChildKey = leftKey;
            minChildIdx = left.idx;
        } else {
            minChildKey = rightKey;
            minChildIdx = right.idx;
        }
    } else if (isSome(&left)) {
        // Otherwise if the left exists then it is the only child so it must be min
        minChildKey = heap->H[left.idx].key;
        minChildIdx = left.idx;
    } else {
        // Otherwise no children exist so we have reached a leaf node and are done
        return Ok();
    }

    // If the min child key is smaller than the curent key then we swap the elements
    // and recursively call Heapify on the new "child" element.
    if (minChildKey < key) {
        Error e = Swap(heap, index, minChildIdx);
        if (!IsOK(&e)) {
            return e;
        }
        return Heapify(heap, minChildIdx);
    }

    return Ok();
}

// Increases key at the index to key of value
Error DecreaseKey(Heap* heap, int index, int value, int flag) {
    // Check for uninitialized heap
    if (heap->H == NULL) {
        return HeapUninitialized();
    }
    // Check that the index is within the valid size of the heap
    if (heap->size - 1 < index) {
        return OutOfBounds(index, heap->size - 1);
    }
    // Get the current key of the element at index
    int currKey = keyAt(heap, index);
    // Make sure the new key value is actually smaller than the current
    if (value > currKey) {
        return KeyGreaterThanCurrent(currKey, value);
    }
    // Print heap before operation if wanted
    if (flag == 1) {
        Error e = printHeap(heap);
        if (!IsOK(&e)) {
            return e;
        }
    }
    // Set the key value of the element to the new desired value
    heap->H[index].key = value;

    // Iterate up the "tree" and swap the current element with its parent
    // until the parent key is less than or equal to the current key or until we reach
    // the top of the "tree"
    int parent = Parent(index);
    while (index > 0 && keyAt(heap, parent) > keyAt(heap, index)) {
        Error e = Swap(heap, parent, index);
        if (!IsOK(&e)) {
            return e;
        }
        index = parent;
        parent = Parent(index);
    }

    // Print state of heap after operation if wanted
    if (flag == 1) {
        Error e = printHeap(heap);
        if (!IsOK(&e)) {
            return e;
        }
    }

    // Return no error if we get here without error
    return Ok();
}

// Helper function for reallocation to find the next power of two greater than n
int nextPowerOfTwo(int n) {
    int temp = 2;
    while (temp <= n) {
        temp *= 2;
    }
    return temp;
}

// Increases heap capacity to the next greatest power of two by reallocating memory
void increaseCapacity(Heap* heap) {
    heap->capacity = nextPowerOfTwo(heap->capacity);
    Element* new_mem = realloc(heap->H, heap->capacity * sizeof(Element));
    heap->H = new_mem;
}

// Inserts an element with key k into the min heap
Error Insert(Heap* heap, int k, int flag) {
    // Check for uninitialized heap
    if (heap->H == NULL) {
        return HeapUninitialized();
    }
    // Print state before operation if wanted
    if (flag == 1) {
        Error e = printHeap(heap);
        if (!IsOK(&e)) {
            return e;
        }
    }

    // The index we will be inserting into (next available)
    int index = heap->size;
    // Increase size and increase heap capacity if needed to support the new size
    heap->size++;
    if (heap->size > heap->capacity) {
        increaseCapacity(heap);
    }

    // Set the memory in the heap's internal array to be an Element with minimum integer key
    heap->H[index] = (Element){INT_MAX};

    // Use DecreaseKey operation on the element with the proper key value
    Error e = DecreaseKey(heap, index, k, 0);
    if (!IsOK(&e)) {
        return e;
    }

    // Print heap state after operation if wanted
    if (flag == 1) {
        e = printHeap(heap);
        if (!IsOK(&e)) {
            return e;
        }
    }

    // Return no error if we get here without error
    return Ok();
}

// Deletes the min element from the heap.
Error DeleteMin(Heap* heap, int flag) {
    // Check for uninitialized heap
    if (heap->H == NULL) {
        return HeapUninitialized();
    }
    // Check that heap size is not 0
    if (heap->size <= 0) {
        return DeleteZeroElements();
    }
    // Print out heap state before operation if wanted
    if (flag == 1) {
        Error e = printHeap(heap);
        if (!IsOK(&e)) {
            return e;
        }
    }
    // Swap min element with last element
    Error e = Swap(heap, 0, heap->size - 1);
    if (!IsOK(&e)) {
        return e;
    }

    // Print removed element
    printf("%d\n", keyAt(heap, heap->size - 1));

    // Decrease heap size to excluded swapped min
    heap->size--;

    // Heapify on root to preserve min heap property
    Heapify(heap, 0);

    // Printout heap state after operation if wanted
    if (flag == 1) {
        e = printHeap(heap);
        if (!IsOK(&e)) {
            return e;
        }
    }

    // Return no error if we got to the end without error
    return Ok();
}
