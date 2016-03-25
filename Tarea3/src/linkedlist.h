
#ifndef llink
#define llink

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include "../_Element.h"


typedef struct Nodo
{
    Element *element; 
    struct Nodo *next;
    struct Nodo *previous;
}Nodo;

typedef struct LinkedList
{
    Nodo *first;
    Nodo *last;
    int size;
}LinkedList;


void list_print(LinkedList *list);
Element *list_get(LinkedList *list, char* iPosition);
void list_del(LinkedList *list, Nodo *nodo);
void list_add(LinkedList *list, Element *ielementToAdd);
void list_destroy(LinkedList *list);
LinkedList *list_init();
void list_alloc_test(LinkedList *list);


void list_alloc_test(LinkedList *list){
    if (list == NULL)
    {
        exit(EXIT_FAILURE);
    }
}

LinkedList *list_init()
{
    LinkedList *list = malloc(sizeof(*list));
    Nodo *nodo = malloc(sizeof(*nodo));

    if (list == NULL || nodo == NULL)
    {
        exit(EXIT_FAILURE);
    }

    nodo->element = NULL;
    nodo->next = NULL;
    nodo->previous = NULL;
    
    list->first = nodo;
    list->last = nodo;
    list->size = 0;

    return list;
}

void list_destroy(LinkedList *list)
{
    list_alloc_test(list);

    Nodo *NodoCurrent, *NodoNext;

    NodoCurrent = list->first;
    while (NodoCurrent != NULL)
    {
        if(NodoCurrent->element != NULL){
            element_destroy(NodoCurrent->element);
        }
        NodoNext = NodoCurrent->next;
        free(NodoCurrent);
        NodoCurrent = NodoNext;
    }
    free(NodoCurrent);
    free(NodoNext);
    free(list);
}

void list_destroy2(LinkedList *list)
{
    Nodo *NodoCurrent, *NodoNext;

    NodoCurrent = list->first;
    while (NodoCurrent != NULL)
    {
        NodoNext = NodoCurrent->next;
        free(NodoCurrent);
        NodoCurrent = NodoNext;
    }
    free(NodoCurrent);
    free(NodoNext);
    free(list);
}


void list_add(LinkedList *list, Element *ielementToAdd)
{
    list_alloc_test(list);

    Nodo *nodo = malloc(sizeof(*nodo));

    if (nodo == NULL)
    {
        exit(EXIT_FAILURE);
    }

    nodo->element = ielementToAdd;
    nodo->next = NULL;

    if (list->size == 0)
    {
        free(list->first);
        nodo->previous = NULL;
        list->first = nodo;

    }
    else
    {
        list->last->next = nodo;
        nodo->previous = list->last;
        
    }
    
    list->last = nodo;
    list->size += 1;
}

void list_del(LinkedList *list, Nodo *nodo)
{

    list_alloc_test(list);

    
    Nodo *NodoCurrent, *NodoPrevious;


    if (list->size == 0)
    {
        printf("ERROR. List is empty, nothing to delete\n");
    }
    else if (list->size == 1)
    {
        NodoCurrent = list->first;
        if (!strcmp(NodoCurrent->element->value,nodo->element->value))
        {

            free(NodoCurrent);
            list->first = NULL;
            list->last = NULL;
            list->size = 0;

        }
        else
        {
            printf("ERROR. Nodo not in list, nothing to delete\n");
        }
    }
    else{


        NodoPrevious = list->first;
        NodoCurrent = NodoPrevious->next;
                    


        if (NodoPrevious->element == nodo->element)
        {
            list->first = NodoCurrent;
            NodoCurrent->previous = NULL;
            list->size -= 1;
            free(NodoPrevious);
        }
        else
        {

                    NodoPrevious->next = nodo->next;
                    list->size -= 1;
                    if (nodo->next == NULL)
                    {
                        list->last = NodoPrevious;
                    }
                    else
                    {
                        nodo->next->previous = NodoPrevious;
                    }
                    free(NodoCurrent);
            
        }
    }
}

Element *list_get(LinkedList *list, char* iPosition)
{
    list_alloc_test(list);

    Nodo *NodoCurrent;

    NodoCurrent = list->first;
    int i = 1;
    while (NodoCurrent!=NULL && NodoCurrent->element != NULL)
    {
        if(!strcmp(NodoCurrent->element->value, iPosition)){
            return NodoCurrent->element;
        }
        NodoCurrent = NodoCurrent->next;
    }
    return NULL;
}


void list_print(LinkedList *list)
{
    list_alloc_test(list);

    Nodo *NodoCurrent;

    printf("{");

    if (list->size > 0)
    {
        NodoCurrent = list->first;
        while (NodoCurrent != NULL)
        {
            printf("%s : %s\n", NodoCurrent->element->value, NodoCurrent->element->codification);
            NodoCurrent = NodoCurrent->next;
            if (NodoCurrent != NULL)
            {
                printf(", ");
            }
        }
    }
    printf("}\n");
}


#endif
