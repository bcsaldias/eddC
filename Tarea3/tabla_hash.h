//hashing con encadenamiento
#ifndef encad
#define encad

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include "_Element.h"
#include "hash.h"
#include "src/array.h"
#include "src/linkedlist.h"
#include "math.h"

typedef struct Tabla_encadenamiento{

	Array *array;

}Tabla_encadenamiento;



Tabla_encadenamiento *tabla_init(int size, double carga_max);
void table_add(Tabla_encadenamiento *tabla, Element *element);
void table_remove(Tabla_encadenamiento *table, LinkedList *list, Nodo *nodo);
void replace(Tabla_encadenamiento *tabla);
void table_destroy(Tabla_encadenamiento *tabla);
//void pizza_lista(Tabla_encadenamiento *tabla, char *tipo);




Tabla_encadenamiento *tabla_init(int size, double carga_max){
	Tabla_encadenamiento *tabla = malloc(sizeof(*tabla));
	if(tabla != NULL){
    	tabla->array=array_init(size, carga_max);
	}
    return tabla;
}

long e_modulo(long a, long b){
	long mod = fmod(a,b);
	if (mod < 0){
		return mod+b;
	}
	return mod;
}



void table_add(Tabla_encadenamiento *tabla, Element *element){
    array_alloc_test(tabla->array);
    
   if(array_double_size_if_full(tabla->array)){									
    						//O(1) cte
    	replace(tabla);		//llama a array_init que toma O(2*capacity) + O(2*capacity)*O(1+lambda)
    }

    long posicion = e_modulo(hash(element->value),tabla->array->capacity);
    Element *e = list_get(tabla->array->content[posicion], element->value);
    if(e!=NULL){
    	e->counter += element->counter;
		element_destroy(element);
    }else{
		list_add(tabla->array->content[posicion], element); //O(1) porque agrega al final
		tabla->array->size++;
	}

}

void table_remove(Tabla_encadenamiento *table, LinkedList *list, Nodo *nodo){
	Element* element = nodo->element;
	list_del(list, nodo);
	element_destroy(element);
	table->array->size--;
}

void replace(Tabla_encadenamiento *tabla){

		Array *array = tabla->array;
		Tabla_encadenamiento *new_table = tabla_init(array->capacity*2, array->carga_max);
	    					//O(2*capacity) por inicializar las listas
	    int i = 0;
	    int k = 0;
	    for(i = 0; i< array->capacity; i++){		//O(2*capacity)
	    	LinkedList *list = array->content[i];
	    	int tamano = list->size;
	    	for(k = 0; k< tamano; k++){				//O(1+ lambda)
	    		Nodo *el = list->first;
	    		table_add(new_table,	el->element);		//O(1) porque agrega al final, puede llamar a replace O(2*capa)
	    		list_del(list, el);		// O(1) porque le paso el Nodoo
	    	}
	    }
        array_destroy(tabla->array);
        tabla->array = new_table->array;
        free(new_table);

}

void table_destroy(Tabla_encadenamiento *tabla){

	array_destroy(tabla->array);
	free(tabla);
}


#endif