#ifndef elementt
#define elementt

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef struct Element{
	char* value;			
	int counter;
	struct Element *hijo0;
	struct Element *hijo1;
	int num_hijo;
	char * codification;
	int type;
}Element;

void print_Tree(Element *raiz);

void print_Tree(Element *raiz){
	if (raiz != NULL){
		print_Tree(raiz->hijo1);
		
		print_Tree(raiz->hijo0);
		if(raiz->codification!=NULL){
			printf("num = %i code = %s value = %s\n", raiz->num_hijo, raiz->codification, raiz->value);			
		}
	}
	
}
/*
Este elemento contiene uno de los colores originales, el que es
metido directamente con su frecuencia al heap.
*/
Element* init_Element(char *value){
	Element *element = malloc(sizeof(Element));
	element->value = malloc(sizeof(char)*strlen(value)+1);
	snprintf(element->value,strlen(value)+1,"%s",value);
	free(value);
	element->num_hijo = -1;
	element->counter = 1;
	element->hijo0 = NULL;
	element->hijo1 = NULL;
	element->codification = NULL;
	element->type = 1;

	return element;
}

/*
Este lemento recibe otro dos, se fusiona y se los queda como hijos.
Los hijos son los 2 elementos sacados del minheap, que van formando
el árbol.
*/

Element* init_Element2(Element *uno, Element *dos){
	char *value0 = uno->value;
	char *value1 = dos->value;

	Element *element = malloc(sizeof(Element));

	element->value = malloc(sizeof(char)*(strlen(value0)+strlen(value1))+1+1);
	snprintf(element->value,strlen(value0)+strlen(value1)+1+1,"%s %s",value0,value1);
	element->num_hijo = -1;
	element->counter = uno->counter + dos->counter;
	element->codification = NULL;
	
	element->type = 2;

	uno->num_hijo = 0;
	element->hijo0 = uno;

	dos->num_hijo = 1;
	element->hijo1 = dos;
	
	return element;
}

void element_destroy(Element *element)
{
	if (element != NULL){
		if(element->value != NULL){
   			free(element->value);
		}
  		if(element->codification != NULL){
   			free(element->codification);
		}  
    	free(element);	
	}

}

#endif