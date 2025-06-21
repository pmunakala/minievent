/*
 * MIT License
 * 
 * File:        min_heap.h
 * Description : Interface for min_heap
 * Author:      Pradeep Kumar Munakala
 * Copyright:   (c) 2025 Pradeep Munakala
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
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
