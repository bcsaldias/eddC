#ifndef hofmm
#define hofmm

#include "heap.h"
#include "src/linkedlist.h"

void asignar_codificacion(Element *element, LinkedList *list, char * space, int u);
void asignar_a_hijos(int i, Element *element, char *prefijo, LinkedList *list, char * space);

void asignar_codificacion(Element *element, LinkedList *list, char * space, int u){
	if (u == 0){
		//este es el caso base por si es solo un color.
		element->codification = malloc(sizeof(char)*2);
		sprintf(element->codification, "%s", "0");
		sprintf(space, "%s", "1");
		list_add(list, element);
	}
	else if (element!= NULL){
		//llamada a asignar hijos
		element->codification = malloc(sizeof(char)*2);
		sprintf(element->codification, "%s", "");
		asignar_a_hijos(0, element->hijo0, "", list, space);
		asignar_a_hijos(1, element->hijo1, "", list, space);
	}
}

void asignar_a_hijos(int i, Element *element, char *prefijo, LinkedList *list, char * space){
	if (element!=NULL){
		if (element->codification == NULL){
			if (element->type == 1){
				list_add(list, element);
			}
			element->codification = malloc(sizeof(char)*(strlen(prefijo) + 2));
			if (i == 0){
				sprintf(element->codification, "%s%s",prefijo,"0");
			}else{
				sprintf(element->codification, "%s%s",prefijo,"1");
			}
		}
		/*se llama recursivamete a los hijos avisando qué bit se deberá
		concatenar en cada caso.
		*/
		asignar_a_hijos(0, element->hijo0, element->codification, list, space);
		asignar_a_hijos(1, element->hijo1, element->codification, list, space);
	}else{
		/*
		para hacerlo eficiente, el separador es el string más 
		pequeño que queda fuera de la codificación. 
		*/
		sprintf(space, "%s%s%s", prefijo, "1", "0");
	}
}

#endif