#ifndef MINHEAP_H
#define MINHEAP_H

typedef int (*compare_cb_t)(void *a, void *B);
typedef struct {
    void *data; // Optional payload
} HeapItem;

typedef struct {
    HeapItem* items;
    int capacity;
    int size;
    compare_cb_t cb;
} MinHeap;

MinHeap* heap_create(int capacity, compare_cb_t cb);
void heap_free(MinHeap* heap);

void heap_push(MinHeap* heap, void* data);
HeapItem heap_pop(MinHeap* heap);
HeapItem heap_peek(MinHeap* heap);
int heap_is_empty(MinHeap* heap);

#endif
