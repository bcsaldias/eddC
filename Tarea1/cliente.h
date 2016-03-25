#ifndef clientee
#define clientee

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

typedef struct Cliente{
	char* name;			//puntero de caracteres porque es un strig.
	char* tipo_pizza;
	int cantidad_pizza;
	int id;
}Cliente;


Cliente *init_Client(char *nombre, char* tipo_pizza, int cantidad_pizza){
	Cliente *client = malloc(sizeof(Cliente));

	client->name = malloc(sizeof(char)*strlen(nombre)+1);
	snprintf(client->name,strlen(nombre)+1,"%s",nombre);

	client->tipo_pizza = malloc(sizeof(char)*strlen(tipo_pizza)+1);
	snprintf(client->tipo_pizza,strlen(tipo_pizza)+1,"%s",tipo_pizza);
	
	client->cantidad_pizza = cantidad_pizza;
	client->id = -1;
	return client;
}

void client_destroy(Cliente *client)
{
    free(client->name);
    free(client->tipo_pizza);
    free(client);
}

int recibir_pizza(Cliente *client){
	client->cantidad_pizza = client->cantidad_pizza -1;
	printf("%s\n",client->name);
	return client->cantidad_pizza;
}

#endif
