/**
 * These are some basic tests for your min-heap implementation. These are by
 * no means comprehensive, and you *MUST* be testing this with additional test
 * cases of your own to ensure correctness.
 * 
 * Feel free to modify this file / add tests, etc. You will not be submitting
 * this.
 * 
 * Compile with:
 *     `make test_minheap`
 *             OR
 *     `gcc test_minheap.c minheap.c -o test_minheap -lm`
 * 
 * and run with:
 *    `./test_minheap`   (or `.\test_minheap.exe` if on Windows)
 * 
 * CSCB63 Winter 2021 - Assignment 2/3
 * (c) Mustafa Quraish
 */

#include "minheap.h"  // Includes ImgUtils.h
#include "unittest.h"

/**
 * Recursively check if the heap property is satisfied.
 */
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

/**
 * Check if the heap looks OK (does not necessarily mean it is fully correct).
 */
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

/**
 * Print out the heap to help debug
 */
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

/*****************************************************************************/

TEST(new_heap) {
  MinHeap *heap = newMinHeap(1024);
  if (heap->numItems != 0) TEST_FAIL("heap->numItems is not correct.\n");
  if (heap->maxSize != 1024) TEST_FAIL("heap->maxSize is not correct.\n");
  for (int i = 0; i < 1024; i++)
    if (heap->indices[i] != -1) TEST_FAIL("heap->indices[%d] != -1\n", i);
}

TEST(in_order_insert) {
  MinHeap *heap = newMinHeap(10);
  for (int i = 0; i < 10; i++) heapPush(heap, i, (double)i);
  for (int i = 0; i < 10; i++)
    if (heap->arr[i].val != i || heap->arr[i].priority != i) 
      TEST_FAIL("Heap values not inserted correctly\n");
}

TEST(reverse_insert) {
  MinHeap *heap = newMinHeap(10);
  for (int i = 9; i >= 0; i--) heapPush(heap, i, (double)i);
  if (!checkHeap(heap)) TEST_FAIL("Failed insert\n");
}

TEST(extract_min_ordered) {
  MinHeap *heap = newMinHeap(10);
  for (int i = 0; i < 10; i++) heapPush(heap, i, (double)i);
  if (!checkHeap(heap)) TEST_FAIL("Failed checkHeap()\n");
  double pri;
  for (int i = 0; i < 10; i++)
    if (heapExtractMin(heap, &pri) != i || pri != i) 
      TEST_FAIL("ExtractMin did not return correct values\n");
}

TEST(extract_min_random) {
  double p[] = {3.9,2.2,7.7,6.5,7.6,8.9,4.6,3.0,8.3,1.9,4.7,2.8,7.3,5.1,1.4};
  MinHeap *heap = newMinHeap(15);
  for (int i = 0; i < 15; i++) heapPush(heap, i, p[i]);
  if (!checkHeap(heap)) TEST_FAIL("Failed checkHeap()\n");
  
  // Sorted indices
  int si[] = {14,9,1,11,7,0,6,10,13,3,12,4,2,8,5};
  double pri;
  for (int i = 0; i < 15; i++)
    if (heapExtractMin(heap, &pri) != si[i] || pri != p[si[i]]) 
      TEST_FAIL("ExtractMin did not return correct values\n");
}

TEST(decrease_priorities_1) {
  MinHeap *heap = newMinHeap(100);
  // Start off with a big priority for all elements
  for (int i = 0; i < 100; i++) heapPush(heap, i, 100000.0);
  // Reduce all their priorities
  for (int i = 0; i < 100; i++) heapDecreasePriority(heap, i, 99-i);

  if (!checkHeap(heap)) TEST_FAIL("Failed checkHeap()\n");
  
  double pri;
  for (int i = 0; i < 100; i++)
    if (heapExtractMin(heap, &pri) != 99-i || pri != i) 
      TEST_FAIL("Decrease Priority didn't assign priorities correctly.\n");
}


int main(int argc, char *argv[]) {
  unit_main(argc, argv);
  return 0;
}