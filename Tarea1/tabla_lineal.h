//hashing con sondeo lineal
#ifndef sondLineal
#define sondLineal

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include "cliente.h"
#include "hash.h"
#include "src/array_cliente.h"
#include "math.h"

typedef struct Tabla_lineal{

	ArrayCliente *array;
	int id;
	int salto;
	int modo;

}Tabla_lineal;


Tabla_lineal *lineal_tabla_init(int size, double carga_max, int salto, int id, int modo);
void lineal_table_add(Tabla_lineal *tabla, Cliente *client, int era);
void lineal_pizza_lista(Tabla_lineal *tabla, char *tipo);
void lineal_replace(Tabla_lineal *tabla);
void lineal_table_add_element(Tabla_lineal *tabla, CElement *element);


Tabla_lineal *lineal_tabla_init(int size, double carga_max, int salto, int id, int modo){
	Tabla_lineal *tabla = malloc(sizeof(*tabla));
	if(tabla != NULL){
    	tabla->array=client_array_init(size, carga_max);
    	tabla->id = id;
    	tabla->salto = salto;
    	tabla->modo = modo;
	}
    return tabla;
}

long modulo(long a, long b){
	long int mod = fmod(a,b);
	if (mod < 0){
		return mod+b;
	}
	return mod;
}


void lineal_table_add(Tabla_lineal *tabla, Cliente *client, int era){
    client_array_alloc_test(tabla->array);
    if(client_array_double_size_if_full(tabla->array)){

    	lineal_replace(tabla);

    }
    
    long posicion = hash(client->tipo_pizza);

    //printf("%s %lu %i %lu\n", client->tipo_pizza, posicion, modulo(posicion,tabla->array->capacity),tabla->array->capacity);

    int vez = 1;
    while(tabla->array->content[modulo(posicion,tabla->array->capacity)]->status==1){
    	if(tabla->modo == 1){
    		posicion+=tabla->salto;
		}else if (tabla->modo == 2){
			posicion+=vez*universal(modulo(posicion,tabla->array->capacity));
		}
		vez++;
    }

    posicion = modulo(posicion,tabla->array->capacity);
   	
    CElement *elemento = tabla->array->content[posicion];
   	elemento->value = client;
   	if(elemento->status==2 || era){
		elemento->ex_status = 2;
   	}
   	elemento->status = 1;

	tabla->id++;
	if(client->id == -1){
		client->id = tabla->id;
	}

	tabla->array->size++;
	//client_array_print(tabla->array);
}

void lineal_table_add_element(Tabla_lineal *tabla, CElement *element){

	client_array_alloc_test(tabla->array);

	if(element->status == 1){
		
		lineal_table_add(tabla, element->value, element->ex_status);
		//free(element);
	}
}

void lineal_replace(Tabla_lineal *tabla){
		ArrayCliente *array = tabla->array;
		Tabla_lineal *new_table = lineal_tabla_init(array->capacity*2, array->carga_max, tabla->salto, tabla->id, tabla->modo);
	   
	    int i = 0;
	    for (i = 0; i < array->capacity; i++)
	    {
	    	lineal_table_add_element(new_table, array->content[i]);
	    }

        //client_array_destroy(tabla->array);
        tabla->array = new_table->array;
        free(new_table);
}

void client_array_destroy_fe(Tabla_lineal *tabla, int c){
    
    if(c){
    	//printf("\n CORTADO \n");
	   	int i = 0;
	    for (i = 0; i < tabla->array->capacity; i++)
	    {
	    	if(tabla->array->content[i]->value != NULL){
	    		client_destroy(tabla->array->content[i]->value);
	    	}
	        free(tabla->array->content[i]);
	    }
	    free(tabla->array->content);
	    free(tabla->array);
    }else{
    	client_array_destroy(tabla->array);
    }
    free(tabla);
}

void lineal_pizza_lista(Tabla_lineal *tabla, char *tipo){

	long posicion = hash(tipo);
	CElement *elemento = tabla->array->content[modulo(posicion,tabla->array->capacity)];
	if(elemento->status==1 && elemento->ex_status==0 && !strcmp(elemento->value->tipo_pizza,tipo) ){
		if(!recibir_pizza(elemento->value)){
			client_destroy(elemento->value);
			elemento->value = NULL;
   			elemento->status = 2;
		}
	}else{
		int menor_id = -1;
		int mayor_posi = modulo(posicion,tabla->array->capacity);


		int vez = 1;
		while(elemento->status!=0){
			elemento = tabla->array->content[modulo(posicion,tabla->array->capacity)];

			if(elemento->value!=NULL && (elemento->value->id < menor_id || menor_id==-1)){
				if(!strcmp(elemento->value->tipo_pizza,tipo))
				{
					menor_id = elemento->value->id;
					mayor_posi =  modulo(posicion,tabla->array->capacity);
				}
			}
			if(tabla->modo == 1){
    			posicion+=tabla->salto;
			}else if (tabla->modo == 2){
				posicion+=vez*universal(modulo(posicion,tabla->array->capacity));
			}
			vez++;

		}
		if(!recibir_pizza(tabla->array->content[mayor_posi]->value)){
			client_destroy(tabla->array->content[mayor_posi]->value);
			tabla->array->content[mayor_posi]->value = NULL;
			tabla->array->content[mayor_posi]->status = 2;
		}

	}
	tabla->array->size--;
}

#endif