#ifndef STATE_LIST
#define STATE_LIST


#include <stdio.h> 
#include <stdlib.h>
#include "labyrinth.h"
#include "list.h"
#include "math.h"
#include "hash.h"



LinkedList *list_init()
{
    // Allocation of the different pointers.
    LinkedList *list = malloc(sizeof(*list));
    Element *element = malloc(sizeof(*element));

    if (list == NULL || element == NULL)
    {
        exit(EXIT_FAILURE);
    }

    // We need to define the pointers.
    element->value = NULL;
    element->next = NULL;
    element->previous = NULL;
    
    list->first = element;
    list->last = element;
    list->size = 0;

    return list;
}

void list_destroy(LinkedList *list)
{

    Element *elementCurrent, *elementNext;

    elementCurrent = list->first;
    while (elementCurrent != NULL)
    {
        if(elementCurrent->value != NULL){
            labyrinth_destroy(elementCurrent->value);
        }
        elementNext = elementCurrent->next;
        free(elementCurrent);
        elementCurrent = elementNext;
    }
    free(elementCurrent);
    free(elementNext);
    free(list);
}

/* ---------------- Manipulation of the data structure ---------------- */

void list_add(LinkedList *list, Labyrinth *iValueToAdd)
{

    // We allocate memory for the new element that will be added.
    Element *element = malloc(sizeof(*element));

    if (element == NULL)
    {
        exit(EXIT_FAILURE);
    }

    // We initialize the element.
    element->value = iValueToAdd;
    // the next element is null because we add at the end of the list.
    element->next = NULL;

    // If the list was empty
    if (list->size == 0)
    {
        // Be careful we need to free the first element because it was 
        // initiated.
        free(list->first);
        element->previous = NULL;
        list->first = element;

    }
    // If the list already contains at least one other element.
    else
    {
        list->last->next = element;
        element->previous = list->last;
        
       // printf("elementos %s %s\n",element->value->name,element->previous->value->name);
    }
    
    list->last = element;
    list->size += 1;
    //printf("labyrinth agregado %s\n", iValueToAdd->name);
}


/////////////////////////////////////////////////
/////////////////////////////////////////////////

Array *array_init(int len)
{
    Array *array = malloc(sizeof(*array));
    array -> size = len;

    LinkedList *list;
    array -> content = malloc(sizeof(struct LinkedList) * array->size);

    int i = 0;
    for(i = 0; i<array->size; i++){
        list = list_init();
        array->content[i] = list;
    }
    return array;
}

int e_modulo(int a, int b){
	int mod = fmod(a,b);
	if (mod < 0){
		return mod+b;
	}
	return mod;
}


void insert_array(Array *array, Labyrinth * value){

	//printf("INSERTANDO !\n");


	// printf("hash: %d\n",hash(value -> lab_ID));
    int posicion = e_modulo(hash(value -> lab_ID), array->size);
	list_add(array->content[posicion], value);
	//printf("INSERTADOO !\n");


}

void destroy_array(Array *array){
	for (int i = 0; i < array->size; ++i)
	{
		list_destroy(array->content[i]);
	}
	free(array->content);
	free(array);
}


#endif