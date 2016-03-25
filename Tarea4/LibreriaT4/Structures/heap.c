#ifndef heapC
#define heapC
#include <stdlib.h>
#include "labyrinth.h"
#include "heap.h"



Heap *heap_init(long len){

    Heap *heap = malloc(sizeof(*heap));
    heap->size = 0;
    heap->capacity = len;
    heap->content = malloc(sizeof(Room *) * heap->capacity);


    return heap;
}

void heap_destroy2(Heap *heap){
	for (int i = 0; i < heap->capacity; ++i)
	{
		if (heap->content[i]  != NULL){
			free(heap->content[i]  -> vecinas);
			free(heap->content[i] );
		}
	}
    free(heap->content);
    free(heap);
}

void heap_destroy(Heap *heap){

    free(heap->content);
    free(heap);
}

Room *xMin(Heap *h){

	Room *new_room = h->content[1]  ;
	h->content[1]  =  h->content[h->size] ;
	h->content[h->size]  = NULL;//new_room;
	/*printf("new room %d %d\n", new_room->x, new_room->y);
	if (h->content[1] )
		printf("1 %d %d\n",h->content[1] ->x, h->content[1] ->y);*/
	//printf("h->size %d %d\n", h->content[h->size] ->x, h->content[h->size] ->y);

	h->size--;

	heapifi(h, 1);
 	return new_room;
}

void decrementKey(Heap *h, int jj){

	int j = jj;
	//printf("LAMADO %d %d \n" , h->size, j);

	while(j < h->size && j > 1 && h->content[j/2] ->distancia > h->content[j] ->distancia){
		//printf("ENTRE %d %d %d\n", h->size, j/2, j);

		Room *aux = h->content[j/2] ;
		h->content[j/2]  = h->content[j] ;
		h->content[j]  = aux;

		h->content[j]  -> position = j;	
		h->content[j/2]  -> position = j/2;

		j = j/2;
	}
}

void heapifi(Heap *h, int j){

	int min_ind = j;

	if(2*j <= h->size && h->content[2*j] ->distancia  < h->content[min_ind] ->distancia  ){
		min_ind = 2*j;
	}
	if(2*j+1 <= h->size && h->content[2*j+1] ->distancia  < h->content[min_ind] ->distancia  ){
		min_ind = 2*j + 1;
	}
	if (min_ind != j){
		Room *aux = h->content[j] ;
		h->content[j]  = h->content[min_ind] ;
		h->content[min_ind]  = aux;

		h->content[j]  -> position = j;
		h->content[min_ind]  -> position = min_ind;

		heapifi(h, min_ind);
	}
}

void basicInsertion(Heap *h, Room *e){
	h->size++;
	h->content[h->size]  = e;
	//printf("Insertion %d\n", e->ID);
}

void buildHeap(Heap *h){
	int i = 0;
	for (i = h->size/2+1; i >= 1; --i)
	{
		heapifi(h, i);
	}
	for (i = 1; i <= h->size; i++)
	{
		if (h->content[i]){
			h->content[i]->position = i;

		}
	}
}



#endif