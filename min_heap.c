#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "min_heap.h"

MinHeap* heap_create(int capacity, compare_cb_t cb) {
    MinHeap* heap = malloc(sizeof(MinHeap));
    heap->capacity = capacity;
    heap->size = 0;
    heap->items = malloc(sizeof(HeapItem) * capacity);
    heap->cb = cb;
    return heap;
}

void heap_free(MinHeap* heap) {
    free(heap->items);
    free(heap);
}

static void swap(HeapItem* a, HeapItem* b) {
    HeapItem temp = *a;
    *a = *b;
    *b = temp;
}
int count = 0;
static void heapify_up(MinHeap* heap, int idx) {
    while (idx > 0) {
        count++;
        int parent = (idx - 1) / 2;
        if (heap->cb(heap->items[idx].data, heap->items[parent].data) < 0) {
            swap(&heap->items[idx], &heap->items[parent]);
            idx = parent;
        } else break;
    }
}

static void heapify_down(MinHeap* heap, int idx) {
    int left, right, smallest;
    while (1) {
        left = 2 * idx + 1;
        right = 2 * idx + 2;
        smallest = idx;

        if (left < heap->size && heap->cb(heap->items[left].data, heap->items[smallest].data) < 0)
            smallest = left;
        if (right < heap->size && heap->cb(heap->items[right].data, heap->items[smallest].data) < 0)
            smallest = right;

        if (smallest != idx) {
            swap(&heap->items[idx], &heap->items[smallest]);
            idx = smallest;
        } else break;
    }
}

void heap_push(MinHeap* heap, void* data) {
    if (heap->size == heap->capacity) {
        heap->capacity *= 2;
        heap->items = realloc(heap->items, sizeof(HeapItem) * heap->capacity);
    }

    heap->items[heap->size].data = data;
    heapify_up(heap, heap->size);
    heap->size++;
    //printHeap(heap);
}

HeapItem heap_pop(MinHeap* heap) {
    if (heap->size == 0) {
        fprintf(stderr, "Heap underflow\n");
        exit(EXIT_FAILURE);
    }
    HeapItem root = heap->items[0];
    heap->items[0] = heap->items[--heap->size];
    heapify_down(heap, 0);
    return root;
}

HeapItem heap_peek(MinHeap* heap) {
    if (heap->size == 0) {
        fprintf(stderr, "Heap is empty\n");
        exit(EXIT_FAILURE);
    }
    return heap->items[0];
}
void printHeap (MinHeap* heap) {
    int i;
    for (i = 0; i < heap->size; i++)
    printf("data %d item %p\n", i, heap->items[i].data);
}
int heap_is_empty(MinHeap* heap) {
    return heap->size == 0;
}
