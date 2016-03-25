/******************************************************************************\
|
|   Author:     Chaumier Pierre-Victor <pvchaumier@uc.cl>
|
\******************************************************************************/

// Include Guards to avoid double declarations
#ifndef array_H
#define array_H

#include <stdio.h> 
#include <stdlib.h>
#include "linkedlist.h"


typedef struct Array
{
    double size; 
    long capacity; 
    LinkedList **content; 
    double carga_max;

}Array;


void array_print(Array *array);
int array_double_size_if_full(Array *array);
void array_destroy(Array *array);
Array *array_init(int len, double c_max);
void array_alloc_test(Array *array);


void array_alloc_test(Array *array){
    if (array == NULL)
    {
        exit(EXIT_FAILURE);
    }
}

Array *array_init(int len, double c_max)
{
    Array *array = malloc(sizeof(*array));

    array_alloc_test(array);

    array->size = 0;
    array->capacity = len;
    array->carga_max = c_max;


        LinkedList *list;
        array->content = malloc(sizeof(list) * array->capacity);
        int i = 0;
        for(i = 0; i<array->capacity; i++){
            list = list_init();
            array->content[i] = list;
        }

    

    if (array->content == NULL)
    {
        printf("ERROR. The content of the array could not be initialize.\n");
        exit(EXIT_FAILURE);
    }

    return array;
}


void array_destroy(Array *array)
{

 	array_alloc_test(array);
    int i = 0;
    for (int i = 0; i < array->capacity; ++i)
    {

        Element *elementCurrent, *elementNext;
        elementCurrent = array->content[i]->first;
        
        while (elementCurrent != NULL)
        {
            if(elementCurrent->value != NULL){
                client_destroy(elementCurrent->value);
            }
            elementNext = elementCurrent->next;
            free(elementCurrent);
            elementCurrent = elementNext;
        }


        free(array->content[i]);
    }
    free(array->content);
    free(array);
}

int array_double_size_if_full(Array *array){

    array_alloc_test(array);

    if (array->size / array->capacity >= array->carga_max)
    {

        return 1;

    }
    return 0;
}




void array_print(Array *array){
    array_alloc_test(array);

    printf("\n[");
    int i;
    for (i = 0; i < array->capacity; i++)
    {
        list_print(array->content[i]);
        if (i != array->capacity - 1)
        {
            printf(", ");
        }
    }
    printf("]\n");
}


#endif
