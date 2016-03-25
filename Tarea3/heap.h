#ifndef element__
#define element__

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include "_Element.h"

typedef struct Heap
{
    int capacity; 
    int size;
    double carga_max;
    Element **content; 

}Heap;

void heap_destroy(Heap *heap);
int heap_double_size_if_full(Heap *heap);
Element *xMin(Heap *h);
void heapifi(Heap *h, int j);
void incrementKey(Heap *h, int j, int k);
void inserObject(Heap *h, Element *element);
void print_heap(Heap *h);
void basicInsertion(Heap *h, Element *e);
void buildHeap(Heap *h);

Heap *heap_init(long len, double c_max){

    Heap *heap = malloc(sizeof(*heap));

    heap->size = 0;
    heap->capacity = len;
    heap->carga_max = c_max;

    //Element *element;
    heap->content = malloc(sizeof(Element) * heap->capacity);

    /*int i = 0;
    for(i = 0; i<heap->capacity; i++){
        heap->content[i] = malloc(sizeof(Element));
        heap->content[i]->value = NULL;
        heap->content[i]->counter = 0;
    }*/
    return heap;
}


Element *xMin(Heap *h){

	Element *to_ret = init_Element(h->content[1]->value);
	to_ret->counter = h->content[1]->counter;
	to_ret->hijo0 = h->content[1]->hijo0;
	to_ret->hijo1 = h->content[1]->hijo1;
	to_ret->num_hijo = h->content[1]->num_hijo;
	to_ret->type = h->content[1]->type;

	h->content[1]->value =  h->content[h->size]->value;
	h->content[1]->counter = h->content[h->size]->counter;
	h->content[1]->type = h->content[h->size]->type;
	h->content[1]->hijo0 = h->content[h->size]->hijo0;
	h->content[1]->hijo1 = h->content[h->size]->hijo1;

	h->content[h->size]->value = NULL;
	h->content[h->size]->counter = 0;
	h->content[h->size]->hijo0 = NULL;
	h->content[h->size]->hijo1 = NULL;
	h->content[h->size]->type = 0;

	h->size--;
	heapifi(h, 1);

	return to_ret;
}

void heapifi(Heap *h, int j){
	int max_ind = j;

	if(2*j <= h->size && h->content[2*j]->counter  < h->content[max_ind]->counter ){
		max_ind = 2*j;
	}
	if(2*j+1 <= h->size && h->content[2*j+1]->counter < h->content[max_ind]->counter ){
		max_ind = 2*j + 1;
	}
	if (max_ind != j){
		Element *aux = h->content[j];
		h->content[j] = h->content[max_ind];
		h->content[max_ind] = aux;
		heapifi(h, max_ind);
	}
}

void buildHeap(Heap *h){
	int i = 0;
	for (i = h->size/2+1; i >= 1; --i)
	{
		heapifi(h, i);
	}
}

void basicInsertion(Heap *h, Element *e){
	h->size++;
	h->content[h->size] = e;
}

void inserObject(Heap *h, Element *element){
	h->size++;
	//element_destroy(h->content[h->size]);
	h->content[h->size] = element;
	incrementKey(h, h->size, element->counter);
}

void incrementKey(Heap *h, int j, int k){
	h->content[j]->counter = k;
	while(j > 1 && h->content[j/2]->counter > h->content[j]->counter){
		Element *aux = h->content[j/2];
		h->content[j/2] = h->content[j];
		h->content[j] = aux;
		j = j/2;
	}
}

int heap_double_size_if_full(Heap *heap){
    if (heap->size / heap->capacity >= heap->carga_max)
    {
        return 1;
    }
    return 0;
}

void heap_destroy(Heap *heap){
    int i = 0;
    for (i = 0; i < heap->size; i++)
    {
    	if (heap->content[i]!=NULL){
    			element_destroy(heap->content[i]);
    	}

    }
    /*for (int j = heap->size; j < heap->capacity; ++j)
    {
    	if (heap->content[i]!=NULL){
    		free(heap->content[j]);
    	}
    }*/
    free(heap->content);
    free(heap);
}


void print_heap(Heap *h){
	/*
	puede ser con capacity
	*/
	printf("\n[");
	int i;
	for (i = 1; i < h->size+1; ++i)
	{
		printf("%s :%i\n", h->content[i]->value, h->content[i]->counter);
		if (i < h->size){
			printf(",");
		}
	}
	printf("]");
}

#endif