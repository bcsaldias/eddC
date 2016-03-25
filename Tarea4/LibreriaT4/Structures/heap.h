#ifndef heapH
#define heapH
#include "labyrinth.h"


typedef struct Heap
{
    int capacity; 
    int size;
    Room ** content; 

}Heap;

Heap *heap_init(long len);
void heap_destroy(Heap *heap);
void heap_destroy2(Heap *heap);
Room *xMin(Heap *h);
void heapifi(Heap *h, int j);
void basicInsertion(Heap *h, Room *e);
void buildHeap(Heap *h);
void decrementKey(Heap *h, int jj);

#endif