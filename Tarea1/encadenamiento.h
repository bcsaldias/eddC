//hashing con encadenamiento
#ifndef encad
#define encad

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include "cliente.h"
#include "hash.h"
#include "src/array.h"
#include "src/linkedlist.h"
#include "math.h"

typedef struct Tabla_encadenamiento{

	Array *array;

}Tabla_encadenamiento;



Tabla_encadenamiento *tabla_init(int size, double carga_max);
void table_add(Tabla_encadenamiento *tabla, Cliente *client);
void table_remove(Tabla_encadenamiento *table, LinkedList *list, Element *element);
void replace(Tabla_encadenamiento *tabla);
void pizza_lista(Tabla_encadenamiento *tabla, char *tipo);




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



void table_add(Tabla_encadenamiento *tabla, Cliente *client){
    array_alloc_test(tabla->array);
    
    if(array_double_size_if_full(tabla->array)){									
    						//O(1) cte
    	replace(tabla);		//llama a array_init que toma O(2*capacity) + O(2*capacity)*O(1+lambda)
    }

    long posicion = e_modulo(hash(client->tipo_pizza),tabla->array->capacity);
	list_add(tabla->array->content[posicion], client); //O(1) porque agrega al final
	tabla->array->size++;

}

void table_remove(Tabla_encadenamiento *table, LinkedList *list, Element *element){
	Cliente* client = element->value;
	list_del(list, element);
	client_destroy(client);
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
	    		Element *el = list->first;
	    		table_add(new_table,	el->value);		//O(1) porque agrega al final, puede llamar a replace O(2*capa)
	    		list_del(list, el);		// O(1) porque le paso el elemento
	    	}
	    }
        array_destroy(tabla->array);
        tabla->array = new_table->array;
        free(new_table);

}

void pizza_lista(Tabla_encadenamiento *tabla, char *tipo){
	long posicion = e_modulo(hash(tipo),tabla->array->capacity);
	LinkedList *list = tabla->array->content[posicion];

	int i = 0;
	for(i = 0; i < list->size; i++){					//O(1+lambda) en promedio peor caso O(n)
		Element *element = list_get(list, i);
		Cliente *client = element->value;
		if(!strcmp(client->tipo_pizza, tipo)){			//acotado por el largo del tipo_pizza (2048)
			if(!recibir_pizza(client)){
				table_remove(tabla, list, element);		//O(1) porque ya tengo al cliente
			}
			break;
		}
	}

}



#endif