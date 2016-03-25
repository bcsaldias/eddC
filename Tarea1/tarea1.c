#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include "cliente.h"
#include "encadenamiento.h"
#include "tabla_lineal.h"

char linea[5000];
Tabla_encadenamiento* tabla0;
Tabla_lineal* tabla1;

int get_line(){
	int bytes = 5000;
	if(fgets(linea, bytes, stdin)!=NULL){
		return 1;
	}
	return 0;
}

Cliente *global;
int readCommand(const char *modo){
	char command[4];
	char nombre[2049];
	int cantidad;
	char tipo[2049];
	scanf("%s", command);

	if(!strcmp(command, "ASK")){
		scanf("%s", nombre);
		scanf("%i", &cantidad);
		scanf("%s", tipo);
		Cliente *client = init_Client(nombre, tipo, cantidad);

		if(!strcmp(modo,"0")){
			table_add(tabla0, client);
		
		}else{
			lineal_table_add(tabla1, client, 0);
		}

	}else if(!strcmp(command, "RDY")){
		scanf("%s", tipo);

		if(!strcmp(modo,"0")){
			pizza_lista(tabla0, tipo);

		}else{
			//printf("pizza lista %s\n",nombre);
			lineal_pizza_lista(tabla1, tipo);

		}

	}else if(!strcmp(command, "END")){
		if(!strcmp(modo,"0")){
			array_destroy(tabla0->array);
			free(tabla0);
		}else{
			client_array_destroy_fe(tabla1, 1);
		}
		return 0;

	}else{
		printf("Comando desconocido !\n");
		exit(1);
	}

	return 1;
}

int modo;


void senal(int sig){
	
	signal(sig, 0);
	if(modo == 0){
		array_destroy(tabla0->array);
		free(tabla0);

	}else{
		client_array_destroy_fe(tabla1, 1);
	}
	exit(0);
}


int main(int argc, char const *argv[])
{
	clock_t start = clock();

	

	if(!strcmp(argv[1],"0")){
		tabla0 = tabla_init(100000, 0.80);
		modo = 0;
	}else if(!strcmp(argv[1],"1")){
		tabla1 = lineal_tabla_init( 500000, 0.70, 1, 0, 1);
		modo = 1;

	}else if(!strcmp(argv[1],"2")){
		tabla1 = lineal_tabla_init( 500000, 0.70, 0, 0, 2);
		modo = 2;
	}


	signal(SIGINT, senal);


	readCommand(argv[1]);
	while(get_line()==1){
		readCommand(argv[1]);
	}			





	//printf("Tiempo transcurrido: %f\n", ((double)clock()-start)/CLOCKS_PER_SEC);
	return 0;
}