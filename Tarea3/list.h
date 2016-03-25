
//Estructura nodo. Aca se almacena una unica llave
//(color o codificacion segun si se esta comprimiendo o descomprimiendo)
//y el valor de retorno (codificacion o color de manera analoga).
//Esta estructura se usa para hacer una lista ligada de llaves (los nodos del skip tree usaran esta estructura)
//No existe una estructura lista de manera formal, sino que se representa
//apuntando al primer nodo de la lista y termna cuando el siguiente nodo es NULL
typedef struct NodoJ{
	struct NodoJ* next;
	char* valor;
	char* valor_retorno;
}NodoJ;

void init_nodo(NodoJ* nodo, char* valor, char* valor_retorno){
	(*nodo).next = NULL;
	(*nodo).valor = valor;
	(*nodo).valor_retorno = valor_retorno;
}

NodoJ* new_nodo(char* valor, char* valor_retorno){
	NodoJ* nodo = malloc(sizeof(NodoJ));
	init_nodo(nodo, valor, valor_retorno);
	return nodo;
}

void nodo_destroy(NodoJ* nodo){
	free(nodo);
}

//Buscar elemento (segun valor que almacena) dentro de una lista
NodoJ* search_value(NodoJ* nodo, char* value){
	NodoJ* actual = nodo;
	while(actual!=NULL && (*actual).valor!=value){
		actual = (*actual).next;
	}
	return actual;	
}

//Buscar indice de un nodo dentro de una lista
int search_index(NodoJ* nodo, NodoJ* nodo_buscado){
	int index= 0;
	NodoJ* actual = nodo;
	while(actual!=NULL && actual!=nodo_buscado){
		actual = (*actual).next;
		index = index + 1;
	}
	if(actual==NULL){
		index = -1;
	}
	return index;	
}

//Dividir lista en dos, en el nodo indicado. 
//El puntero que se retorna es el inicio de la nueva lista. 
NodoJ* split_nodo(NodoJ** nodo, NodoJ* cut){
	NodoJ* actual = *nodo;
	if(nodo!=NULL && cut!=NULL){
		NodoJ* segundo;
		if(actual==cut){
			segundo = (*actual).next;
			*nodo = NULL;
		}
		else{
			while((*actual).next!=NULL && (*actual).next!=cut){
				actual = (*actual).next;
			}
			segundo = (*(*actual).next).next;
			(*actual).next = NULL;
		}
		return segundo;
	}
	else{
		printf("CORTE O NODO INICIAL NULO\n");
		return NULL;
	}

}

//Se inserta nodo a lista segun su valor
int insert_nodo(NodoJ** root, NodoJ* nodo){
	int index = 0;
	if(*root==NULL){
		*root = nodo;
	}
	else{
		NodoJ* actual = *root;
		if(strcmp((*nodo).valor,(*actual).valor)<0){
			NodoJ* next = *root;
			*root = nodo;
			(*nodo).next = next;
		}
		else{
			index = index + 1;
			while((*actual).next!=NULL && strcmp((*nodo).valor,(*(*actual).next).valor)>0){
				actual = (*actual).next;
				index = index + 1;
			}
			NodoJ* next = (*actual).next;
			(*actual).next = nodo;
			(*nodo).next = next;
		}
	}
	return index;
}

void print_nodos(NodoJ* nodo){
	NodoJ* actual = nodo;
	while(actual!=NULL){
		printf("%s ,", (*actual).valor);
		actual = (*actual).next;
	}
	printf("\n");
}

void destroy_list(NodoJ* root){
	NodoJ* actual = root;
	NodoJ* next = NULL;
	while(actual!=NULL){
		next = (*actual).next;
		free(actual);
		actual = next;
	}
}

