#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#include "heap.h"

#define PRINT_TREE

Element Elem(int key) {
    return (Element){key};
}

int Parent(int index) {
    return floor(((float)index - 1.0) / 2.0);
}

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

int isSome(MaybeElement* elem) {
    return elem->exists;
}

Heap Initialize(int n) {
    Element* H = malloc(sizeof(Element) * n);
    Heap heap = {
        .capacity = n,
        .size = 0,
        .H = H,
    };
    return heap;
}

int keyAt(Heap* heap, int index) {
    return heap->H[index].key;
}

void printRaw(Heap* heap) {
    for (int i = 0; i < heap->size; i++) {
        printf("%d, ", heap->H[i].key);
    }
    printf("\n");
}

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

void printHeap(Heap* heap) {
    printf("Capacity: %d\n", heap->capacity);
    printf("Size: %d\n", heap->size);
    printf("Keys: \n");
    printRaw(heap);
    #ifdef PRINT_TREE
    printf("Tree View: \n");
    printPrettyTree(heap);
    #endif
}

Error BuildHeap(Heap* heap, Element* A, int n, int flag) {
    if (heap->capacity < n) {
        return OverCapacity(n, heap->capacity);
    }
    memcpy(heap->H, A, n * sizeof(Element));
    heap->size = n;

    int first = floor((float)n/2.0) - 1;
    for (int i = first; i >= 0; i--) {
        Error e = Heapify(heap, i);
        if (!IsOK(&e)) {
            return e;
        }
        if (flag == 2) {
            printRaw(heap);
        }
    }

    if (flag == 1) {
        printHeap(heap);
    }

    return Ok();
}

Error Swap(Heap* heap, int first, int second) {
    if (heap->size < first) {
        return OutOfBounds(first, heap->size - 1);
    }
    if (heap->size < second) {
        return OutOfBounds(second, heap->size - 1);
    }
    Element temp = heap->H[first];
    heap->H[first] = heap->H[second];
    heap->H[second] = temp;
    return Ok();
}

Error Heapify(Heap* heap, int index) {
    if (heap->size < index) {
        return OutOfBounds(index, heap->size - 1);
    }

    int key = keyAt(heap, index);

    int maxChildKey;
    int maxChildIdx;

    MaybeElement left = LeftChild(heap, index);
    MaybeElement right = RightChild(heap, index);

    if (isSome(&right)) {
        int leftKey = heap->H[left.idx].key;
        int rightKey = heap->H[right.idx].key;
        if (leftKey > rightKey) {
            maxChildKey = leftKey;
            maxChildIdx = left.idx;
        } else {
            maxChildKey = rightKey;
            maxChildIdx = right.idx;
        }
    } else if (isSome(&left)) {
        maxChildKey = heap->H[left.idx].key;
        maxChildIdx = left.idx;
    } else {
        return Ok();
    }

    if (maxChildKey > key) {
        Error e = Swap(heap, index, maxChildIdx);
        if (!IsOK(&e)) {
            return e;
        }
        return Heapify(heap, maxChildIdx);
    }

    return Ok();
}

Error IncreaseKey(Heap* heap, int index, int value, int flag) {
    if (heap->size < index) {
        return OutOfBounds(index, heap->size - 1);
    }
    int currKey = keyAt(heap, index);
    if (index < currKey) {
        return KeyLessThanCurrent(currKey, value);
    }

    if (flag == 1) {
        printf("Heap before IncreaseKey:\n");
        printHeap(heap);
    }

    heap->H[index].key = value;

    int parent = Parent(index);
    while (index > 0 && keyAt(heap, parent) < keyAt(heap, index)) {
        Error e = Swap(heap, parent, index);
        if (!IsOK(&e)) {
            return e;
        }
        index = parent;
        parent = Parent(index);
    }

    if (flag == 1) {
        printf("Heap after IncreaseKey:\n");
        printHeap(heap);
    }

    return Ok();
}

void increaseCapacity(Heap* heap) {
    heap->capacity *= 2;
    Element* new_mem = realloc(heap->H, heap->capacity * sizeof(Element));
    heap->H = new_mem;
}

Error Insert(Heap* heap, int k, int flag) {
    if (flag == 1) {
        printf("Heap before insertion:\n");
        printHeap(heap);
    }

    int index = heap->size;
    heap->size++;
    if (heap->size > heap->capacity) {
        increaseCapacity(heap);
    }

    heap->H[index] = (Element){INT_MIN};

    Error e = IncreaseKey(heap, index, k, 0);
    if (!IsOK(&e)) {
        return e;
    }

    if (flag == 1) {
        printf("Heap after insertion:\n");
        printHeap(heap);
    }

    return Ok();
}

Error DeleteMax(Heap* heap, int flag) {
    if (flag == 1) {
        printf("Heap before deletion:\n");
        printHeap(heap);
    }
    Error e = Swap(heap, 0, heap->size - 1);
    if (!IsOK(&e)) {
        return e;
    }
    heap->size--;
    Heapify(heap, 0);
    if (flag == 1) {
        printf("Heap after deletion:\n");
        printHeap(heap);
    }
    return Ok();
}
