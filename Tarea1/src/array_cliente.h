#ifndef array_C
#define array_C

#include <stdio.h> 
#include <stdlib.h>
#include "../cliente.h"


typedef struct CElement
{
    Cliente *value; 
    int status; //0 vacio, 1 ocupado, 2 vacio por eliminacion
    int ex_status;

}CElement;


typedef struct ArrayCliente
{
    double size; 
    long capacity; 
    CElement **content; 
    double carga_max;

}ArrayCliente;


ArrayCliente *client_array_init(long len, double c_max);
void client_array_alloc_test(ArrayCliente *array);
void client_array_destroy(ArrayCliente *array);
int client_array_double_size_if_full(ArrayCliente *array);
void client_array_print(ArrayCliente *array);



ArrayCliente *client_array_init(long len, double c_max){

    ArrayCliente *array = malloc(sizeof(*array));
    client_array_alloc_test(array);

    array->size = 0;
    array->capacity = len;
    array->carga_max = c_max;


        	CElement *element;
        	array->content = malloc(sizeof(element) * array->capacity);

        int i = 0;
        for(i = 0; i<array->capacity; i++){
            array->content[i] = malloc(sizeof(*element));
            array->content[i]->value = NULL;
            array->content[i]->status = 0;
            array->content[i]->ex_status = 0;
        }

    return array;
}


int client_array_double_size_if_full(ArrayCliente *array){
	client_array_alloc_test(array);
    if (array->size / array->capacity >= array->carga_max)
    {
        //printf("CAAAAAMBIO %f %lu\n", array->size, array->capacity);


        return 1;

    }
    return 0;
}


void client_array_alloc_test(ArrayCliente *array){
    if (array == NULL)
    {
        exit(EXIT_FAILURE);
    }
}


void client_array_destroy(ArrayCliente *array){
	//client_array_alloc_test(array);

    int i = 0;
    for (i = 0; i < array->capacity; i++)
    {
        free(array->content[i]);
    }
    free(array->content);
    free(array);
}

void client_array_print(ArrayCliente *array){
	client_array_alloc_test(array);

    printf("\n[");
    int i;
    for (i = 0; i < array->capacity; i++)
    {

        if (i != array->capacity - 1)
        {
            printf(", ");
        }
    }
    printf("]\n");
}

#endif