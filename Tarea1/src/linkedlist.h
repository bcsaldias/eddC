/******************************************************************************\
|
|   Author:     Chaumier Pierre-Victor <pvchaumier@uc.cl>
|
\******************************************************************************/

// Include Guards to avoid double declarations
#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include "../cliente.h"

// We will implement a doubly linked list. Each element points towards its 
// previous and next element in the list.
// Notice that you can easily modify iy to get a Queue or a Stack !

// In this implementation, we assume that the list will only contain natural
// integer (otherwise, we would encounter issues in the get function. Send a
// mail if you want more details)

// Defining an element type
typedef struct Element
{
    Cliente *value; 
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


void list_print(LinkedList *list);
Element *list_get(LinkedList *list, int iPosition);
void list_del(LinkedList *list, Element *element);//Cliente *iValueToDel);
void list_add(LinkedList *list, Cliente *iValueToAdd);
void list_destroy(LinkedList *list);
LinkedList *list_init();
void list_alloc_test(LinkedList *list);

/* ---------------- Memory manipulation for data structure ---------------- */

void list_alloc_test(LinkedList *list){
    // Test if the list is allocated in memory, if not returns with error
    if (list == NULL)
    {
        exit(EXIT_FAILURE);
    }
}

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
    list_alloc_test(list);

    Element *elementCurrent, *elementNext;

    elementCurrent = list->first;
    while (elementCurrent != NULL)
    {
        if(elementCurrent->value != NULL){
            client_destroy(elementCurrent->value);
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

void list_add(LinkedList *list, Cliente *iValueToAdd)
{
    list_alloc_test(list);

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
    //printf("cliente agregado %s\n", iValueToAdd->name);
}

//Cliente *iValueToDel)
void list_del(LinkedList *list, Element *element)
{

    list_alloc_test(list);

    
    Element *elementCurrent, *elementPrevious;


    // List empty
    if (list->size == 0)
    {
        printf("ERROR. List is empty, nothing to delete\n");
    }
    // List contains only one element
    else if (list->size == 1)
    {
        elementCurrent = list->first;
        if (!strcmp(elementCurrent->value->name,element->value->name))
        {

            free(elementCurrent);
            list->first = NULL;
            list->last = NULL;
            list->size = 0;

        }
        else
        {
            printf("ERROR. Element not in list, nothing to delete\n");
        }
    }
    // List has at least two elements
    else{


        elementPrevious = list->first;
        elementCurrent = elementPrevious->next;
                    


        if (elementPrevious->value == element->value)
        {
            list->first = elementCurrent;
            elementCurrent->previous = NULL;
            list->size -= 1;
            //client_destroy(elementCurrent->value);
            free(elementPrevious);
        }
        // Element to delete is not the first
        else
        {

                    elementPrevious->next = element->next;
                    list->size -= 1;
                    // if we are deleting the last element
                    if (element->next == NULL)
                    {
                        list->last = elementPrevious;
                    }
                    else
                    {
                        element->next->previous = elementPrevious;
                    }
                   // client_destroy(elementCurrent->value);
                    free(elementCurrent);
            
        }
    }
}

Element *list_get(LinkedList *list, int iPosition)
{
    list_alloc_test(list);

    Element *elementCurrent;
   
    // Check if position asked is present in the list.
    if (iPosition > list->size - 1)
    {
        printf("ERROR. The list only contains %d elements.\n", list->size);
        return NULL;
    }
    else
    {
        // We need to go through the list to acced to the iPosition element.
        int i;
        elementCurrent = list->first;
        for (i = 0; i < iPosition; i++)
        {
            elementCurrent = elementCurrent->next;
        }
        // Now we reached the iPosition element of the list.
        return elementCurrent;//->value;
    }
}


void list_print(LinkedList *list)
{
    // We will go through the list and print each of the element.s
    list_alloc_test(list);

    Element *elementCurrent;

    printf("{");

    if (list->size > 0)
    {
        elementCurrent = list->first;
        while (elementCurrent != NULL)
        {
            printf("%s", elementCurrent->value->name);
            elementCurrent = elementCurrent->next;
            if (elementCurrent != NULL)
            {
                printf(", ");
            }
        }
    }
    printf("}\n");
}


#endif
