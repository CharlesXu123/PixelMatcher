/**
 *                        Min-Heaps
 * 
 * Please make sure you read the blurb below to ensure you understand how we 
 * are implementing min-heaps here and what assumptions / requirements are
 * being made.
 * 
 * You shouldn't change any of the code here. You will NOT be submitting this 
 * file as part of your assignment. You don't need to declare any helper 
 * functions here since they are not going to be exposed to any of the other 
 * files.
 * 
 * CSCB63 Winter 2021 - Assignment 2/3
 * (c) Mustafa Quraish
 */

#ifndef __MINHEAP_H__
#define __MINHEAP_H__

#include<stdlib.h>
#include<stdio.h>
#include<math.h>

/******************************** <READ ME> ***********************************

In order to be able to implement the `decrease_priority()` function for a heap
in O(log n) time, we need to be able to able to retrieve the index of an 
the value in the heap in O(1) time. In a naive implementation, this is not 
possible to do without a linear scan of the heap (which is O(n)).  

In our case, the keys (values) will only be integer values from [0-MAXSIZE], 
where  MAXSIZE is the total number of pixels in an image [for more on this, 
look at `marcher.c` ]. So, we can store an array of MAXSIZE integers inside 
the heap such that: 

    heap->indices[v] == index in the heap array of the value `v`

Initially we will set all the values in this indices array to -1, and we will
update the corresponding indices whenever a new element is inserted, removed,
or swapped around as a result of the heap-operations.

******************************** </READ ME> **********************************/

// One node of the heap
typedef struct {
  int val;
  double priority;
} HeapElement;

// Array representation of the heap, with some metadata
typedef struct {
  int numItems;       // Number of items currently in the heap
  int maxSize;           // Total (maximum) size of the heap 
  
  HeapElement *arr;   // The actual array representing the minHeap
  int *indices;       // indices[key] = index of key in `arr`, or -1
} MinHeap;

// Allocate and free
MinHeap *newMinHeap(int maxSize);
void freeHeap(MinHeap *heap);

// Core heap functions...
void heapPush(MinHeap *heap, int val, double priority);
int heapExtractMin(MinHeap *heap, double *priority);
void heapDecreasePriority(MinHeap *heap, int val, double priority);


#endif // __MINHEAP_H__
