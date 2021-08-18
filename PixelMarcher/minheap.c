/**
 *                        Min-Heaps
 * 
 * This is the A2 section of your coding assignment. You will only submit this
 * along with the A3 section when A3 is due, but it is recommended that you
 * complete this early on so you have time to work on second part of this
 * assignment (in `marcher.c` ).
 * 
 * Please make sure you read the blurb in `minheap.h` to ensure you understand 
 * how we are implementing min-heaps here and what assumptions / requirements 
 * are being made.
 * 
 * CSCB63 Winter 2021 - Assignment 2/3
 * (c) Mustafa Quraish
 */

#include "minheap.h"

/**
 * Allocate a new min heap of the given size.
 * 
 * TODO: 
 *  Allocate memory for the `MinHeap` object, and the 2 arrays inside it.
 *  `numItems` should initially be set to 0, and all the indices should be
 *   set to -1 to start with (since we don't have anything in the heap).
 */
MinHeap *newMinHeap(int size) {
  MinHeap* new = malloc(1*sizeof(MinHeap));
  new->numItems = 0;
  new->maxSize = size;
  new->indices = malloc(size*sizeof(int));
  new->arr = malloc(size*sizeof(HeapElement));
  for (int i = 0; i<size; i++){
    new->indices[i] = -1;
  }
  return new; // Allocate and return heap.
}

/**
 * Swaps elements at indices `a` and `b` in the heap, and also updates their
 * indices. Assumes that `a` and `b` are valid.
 * 
 * NOTE: This is already implemented for you, no need to change anything.
 */
void swap(MinHeap *heap, int a, int b) {
  // Swap the elements
  HeapElement temp = heap->arr[a];
  heap->arr[a] = heap->arr[b];
  heap->arr[b] = temp;

  // Refresh their indices
  heap->indices[heap->arr[a].val] = a;
  heap->indices[heap->arr[b].val] = b;
}

int find_parent(int index){
  if (index == 0){
    return 0;
  }
  return ((index + 1) / 2) - 1;
}

int find_left_c(int index, int numItems){
  int l_index = ((index + 1) * 2) - 1;
  if (l_index >= numItems){
    return index;
  }
  return l_index;
}

int find_right_c(int index, int numItems){
  int r_index = ((index + 1)*2 + 1) - 1;
  if (r_index >= numItems){
    return index;
  }
  return r_index;
}

void percolate(MinHeap* heap, int v_i){
  while (heap->arr[find_parent(v_i)].priority > heap->arr[v_i].priority && v_i != 0){
    swap(heap, v_i, find_parent(v_i));
    v_i = find_parent(v_i);
  }
}

/**
 * Add a value with the given priority into the heap.
 * 
 * TODO: Complete this function, and make sure all the relevant data is updated
 *      correctly, including the `indices` array if you move elements around. 
 *      Make sure the heap property is not violated. 
 * 
 * You may assume the value does not already exist in the heap, and there is
 * enough space in the heap for it.
 */
void heapPush(MinHeap *heap, int val, double priority) {
  if (heap->maxSize == heap->numItems){
    fprintf(stderr, "Error: heap is full\n");
    return;
  }
  if (heap->indices[val] != -1){
    fprintf(stderr, "Error: val already in the heap");
    return;
  }
  int v_i = heap->numItems;
  heap->arr[v_i].val = val;
  heap->arr[v_i].priority = priority;
  heap->indices[val] = v_i;
  heap->numItems++;
  //percolate v up
  percolate(heap, v_i);
  return; // Push value to heap before returning
}

int min_index(MinHeap *heap, int p, int l, int r){
  double count[3] = {heap->arr[p].priority, heap->arr[l].priority, heap->arr[r].priority};
  int i_arr[3] = {p, l, r};
  double min = heap->maxSize + 1;
  int index;
  for (int i = 0; i<3; i++){
    if (count[i] < min){
      min = count[i];
      index = i;
    }
  }
  return i_arr[index];
}

void heapify(MinHeap* heap, int v_i){
  while (min_index(heap, v_i, find_left_c(v_i, heap->numItems), find_right_c(v_i, heap->numItems)) != v_i){
    int c_i = min_index(heap, v_i, find_left_c(v_i, heap->numItems), find_right_c(v_i, heap->numItems));
    swap(heap, v_i, c_i);
    v_i = c_i;
  }
}

/**
 * Extract and return the value from the heap with the minimum priority. Store
 *  the priority for this value in `*priority`. 
 * 
 * For example, if `10` was the value with the lowest priority of `1.0`, then
 *  you would have something that is equivalent to:
 *      
 *        *priority = 1.0;
 *        return 10;
 * 
 * TODO: Complete this function, and make sure all the relevant data is updated
 *      correctly, including the `indices` array if you move elements around. 
 *      Make sure the heap property is not violated. 
 * 
 * You may assume there is at least 1 value in the heap.
 */
int heapExtractMin(MinHeap *heap, double *priority) {
  int val = heap->arr[0].val;
  *priority = heap->arr[0].priority;  // Set correct priority 
  //set v to the root
  swap(heap, 0, heap->numItems - 1);
  heap->indices[val] = -1;
  heap->numItems--; //update numItems
  int v_i = 0;
  //heapify
  heapify(heap, v_i);
  return val;         // Return correct value
}

/**
 * Decrease the priority of the given value (already in the heap) with the
 * new priority.
 * 
 * NOTE: You will find it helpful here to first get the index of the value
 *       in the heap from the `indices` array.
 * 
 * TODO: Complete this function, and make sure all the relevant data is updated
 *      correctly, including the `indices` array if you move elements around. 
 *      Make sure the heap property is not violated. 
 * 
 * You may assume the value is already in the heap, and the new priority is
 *  smaller than the old one (caller is responsible for ensuring this).
 */
void heapDecreasePriority(MinHeap *heap, int val, double priority) {
  if (heap->indices[val] == -1){
    fprintf(stderr, "Error: value not in the heap");
  }
  int v_i = heap->indices[val];
  heap->arr[v_i].priority = priority;
  percolate(heap, v_i);
  return;   // Decrease priority before return
}

/**
 * Free the data for the heap. This won't be marked, but it is always good
 * practice to free up after yourself when using a language like C.
 */
void freeHeap(MinHeap *heap) {
  free(heap->arr);
  free(heap->indices);
  free(heap);
  return;   // Free heap and it's arrays
}

/**
void printHeap(MinHeap *heap) {
  printf("Heap: (numItems = %d / size = %d)\n", heap->numItems, heap->maxSize);
  printf("  arr = [");
  for (int i=0; i < heap->numItems; i++) {
    printf("(%d, %f), ", heap->arr[i].val, heap->arr[i].priority);
  }
  printf("]\n  ind = [");
  for (int i=0; i < heap->maxSize; i++) {
    printf("%d, ", heap->indices[i]);
  }
  printf("]\n");
}


int checkRec(MinHeap *heap, int curIdx, double minp) {
  int correct = 1;
  double pr = heap->arr[curIdx].priority;
  if (pr < minp) {
    printf("arr[%d].priority = %f is less than some parent (%f) \n", curIdx, pr, minp);
    correct = 0;
  }

  int lchild = (curIdx + 1) * 2;
  int rchild = lchild - 1;

  if (lchild < heap->numItems) correct = correct && checkRec(heap, lchild ,pr);
  if (rchild < heap->numItems) correct = correct && checkRec(heap, rchild ,pr);
  return correct;
}

static int checkHeap(MinHeap *heap) {
  int correct = 1;

  // Check if the indices match up
  for (int i = 0; i < heap->maxSize; i++)
    if (heap->indices[i] >= 0)
      if (heap->arr[heap->indices[i]].val != i)
        printf("indices[%d] is not the right index.\n", i), correct = 0;

  // Recursively cheap heap property
  return correct && checkRec(heap, 0, heap->arr[0].priority);
}

int main(){
  double p[] = {3.9,2.2,7.7,6.5,7.6,8.9,4.6,3.0,8.3,1.9,4.7,2.8,7.3,5.1,1.4};
  MinHeap *heap = newMinHeap(15);
  for (int i = 0; i < 15; i++) heapPush(heap, i, p[i]);
  
  printHeap(heap);
  // Sorted indices
  int si[] = {14,9,1,11,7,0,6,10,13,3,12,4,2,8,5};
  double pri;
  for (int i = 0; i < 15; i++){
    int true = checkHeap(heap);
    printHeap(heap);
    int val = heapExtractMin(heap, &pri);
  }
}
*/