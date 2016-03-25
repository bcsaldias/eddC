#ifndef STATE_LIST_H
#define STATE_LIST_H

#include "labyrinth.h"

// Defining an element type
typedef struct Element
{
    Labyrinth *value; 
    struct Element *next;
    struct Element *previous;
}Element;

// Defining the general data structure
typedef struct LinkedList
{
    Element *first;
    Element *last;
    int size;
}LinkedList;


/*
Structure to save states
*/
typedef struct Array
{
    int size; 
    struct LinkedList **content;

}Array;

int e_modulo(int a, int b);
Array *array_init(int len);

void destroy_array(Array *array);
void insert_array(Array *array, Labyrinth * value);


void list_add(LinkedList *list, struct labyrinth *iValueToAdd);
void list_destroy(LinkedList *list);
LinkedList *list_init();

#endif