

#include <sys/types.h>
#include <time.h>

typedef struct Stree{
	int height;
	NodoJ_tree* root;
	NodoJ_tree* illegal;
}Stree;

//Crea la raiz del arbol. Nivel 1, con un hijo que contiene un valor ilegal (sera la hoja mas abajo y a la izquierda del arbol)
NodoJ_tree* new_root(){
	NodoJ_tree* root = new_nt(1);
	char* il = malloc(2*sizeof(char));
	sprintf(il,"%s", "");
    NodoJ_tree* illegal = new_leaf(il, NULL);
    insert_child(root, illegal, 0);
    return root;
}

//Divide la lista de los hijos de un nodo del arbol.
NodoJ_tree* split_children(NodoJ_tree* nt,int index){
	NodoJ_tree* child = (*nt).child;
	if(child==NULL){
		printf("NO SE PUEDEN DIVIDIR LOS HIJOS: NO TIENE\n");
		return NULL;
	}
	else{
		int i;
		for(i=0; i< index; i++){
			if(child==NULL){
				printf("HIJOS INSUFICIENTES: INCONSISTENCIA\n");
			}
			else{
				child = (*child).next;				
			}
		}
		NodoJ_tree* segundo = (*child).next;
		(*child).next = NULL;
		return segundo;
	}
}

//Le asigna una lista de hijos a un nodo del arbol
void adopt_children(NodoJ_tree* father){
	NodoJ_tree* child = (*father).child;
	while(child!=NULL){
		(*child).father=father;
		child = (*child).next;
	}
}

//Divide un nodo del arbol. Se le entrega una llave. La localiza entre su lista de llaves y divide el nodo en dos a partir de ahi.
//Divide la lista de hijos y la lista de llaves en dos y los reasigna a los nodos.
NodoJ_tree* split_nt(NodoJ_tree* nt, char* value){
	NodoJ* nodo_split = search_value((*nt).keys, value);
	int index = search_index((*nt).keys, nodo_split);
	if(nodo_split!=NULL){
		NodoJ* second_keys = split_nodo(&((*nt).keys), nodo_split);
		NodoJ_tree* second_children = split_children(nt, index);
		NodoJ_tree* second_nt = new_nt((*nt).height);
		(*second_nt).keys = second_keys;
		(*second_nt).child = second_children;
		adopt_children(second_nt);
		(*second_nt).father = (*nt).father;
	}
	else{
		printf("NO ENCONTRADO\n");
	}
}
//Metodo split del skip tree. Se hace split segun la llave que se entrega como parametro.
//Segun la situacion, se crea un padre o no un padre. Se inserta la llave en el padre y se divide
//el nodo al que pertenecia en dos.
void split(NodoJ_tree* nt, char* value, Stree* stree){
	NodoJ* nodo_split = search_value((*nt).keys, value);
	NodoJ_tree* hijo0 = nt;
	NodoJ_tree* hijo1 = split_nt(nt, value);
	(*hijo1).next = (*hijo0).next;
	(*hijo0).next = hijo1;
	(*nodo_split).next = NULL;
	if(nt == (*stree).root){
		(*stree).height = (*stree).height + 1;
		(*stree).root = new_nt((*stree).height);
		NodoJ_tree* root = (*stree).root;
		(*root).child = hijo0;
		(*hijo0).father=(*stree).root;
		(*hijo1).father=(*stree).root;
		insert_nodo(&(*root).keys, nodo_split);
	}	
	else{
		insert_nodo(&((*(*nt).father).keys), nodo_split);
	}
}

void init_stree(Stree* stree){
	(*stree).height = 1;
    (*stree).root = new_root();
    (*stree).illegal = (*(*stree).root).child;
}

Stree* new_stree(){
	srand(time(NULL));
	Stree* new_stree = malloc(sizeof(Stree));
	init_stree(new_stree);
	return new_stree; 
}

//Destruye un nodo del arbol
void nt_destroy(NodoJ_tree* root){
	NodoJ_tree* actual = root;
	NodoJ_tree* next = NULL;
	while(actual!=NULL){
		next = (*actual).next;
		if((*actual).child!=NULL){
			nt_destroy((*actual).child);	
		}	
		if((*actual).keys!=NULL){
			destroy_list((*actual).keys);
		}		
		free(actual);
		actual = next;
	}	
}

//Destruye el arbol completo
void tree_destroy(Stree* stree){
	NodoJ_tree* illegal = (*stree).illegal;
	free((*(*illegal).keys).valor);
	nt_destroy((*stree).root);
	free(stree);

}

//Metodo percolar del arbol
//Retorna el nodo del arbol donde deberia ser ingresada la nueva llave que se ingresara al arbol
NodoJ_tree* percolate(Stree* stree, char* value){
	NodoJ_tree* actual = (*stree).root;
	while((*actual).height>1){
		NodoJ_tree* child_actual = (*actual).child;
		NodoJ* key_actual = (*actual).keys;
		while(key_actual!=NULL && strcmp(value,(*key_actual).valor)>0){
			child_actual = (*child_actual).next;
			key_actual= (*key_actual).next;
		} 
		actual = child_actual;
	}
	return actual;
}

//Metodo attach del arbol. Localiza el nodo destino donde sera anadida la nueva llave y luego la agrega.
NodoJ_tree* stree_attach_value(Stree* stree, char* value, char* value_retorno){
	NodoJ_tree* destino = percolate(stree, value);
	nt_attach_value(destino, value, value_retorno);
	return destino;	
}

//Determina de manera aleatoria el nivel donde quedara la nueva llave que se ingresara.
int nivel(){
	float q = 0.5;
	int level = 1;
	float r = ((float)rand())/RAND_MAX;
	//printf("%lf\n", r);
	while(r<q){
		level = level + 1;
		r = ((float)rand())/RAND_MAX;
		//printf("%lf\n", r);
	}
	//printf("Nivel: %d\n", level);
	return level;
}

//Anade una llave o valor al arbol
Stree* stree_add_value(Stree* stree, char* value, char* value_retorno){
	NodoJ_tree* destino = stree_attach_value(stree, value, value_retorno);
	//printf("_________Anadido valor: %s___________\n", value);
	//nt_to_string((*stree).root);
	//Calcular con probabilidades cuantos niveles va a subir
	int level = nivel();
	//int level = nivel(stree);
	int i;
	for(i=0;i<level-1;i++){
		split(destino, value, stree);
		//printf("--------split---------\n");
		//printf("Valor: %s\n", value);
		//nt_to_string((*stree).root);
		//printf("--------split fin---------\n");
		destino = (*destino).father;
	}
	//nt_to_string((*stree).root);
}

//Busca una llave y si la encuentra, retorna el nodo que contiene a esa llave (OJO: es del tipo nodo, no del tipo nodo de arbol)
NodoJ* search_key(NodoJ_tree* root, char* value){
	if((*root).height>0){
		//printf("Altura mayor a 0\n");
		NodoJ_tree* down = (*root).child;
		NodoJ* key = (*root).keys;
		//printf("Valor incial: %s\n", (*key).valor);
		while(key!=NULL && strcmp(value,(*key).valor)>0){
			//printf("Valor actual:%s\n",(*key).valor);
			down = (*down).next;
			key = (*key).next;
		}
		if(key!=NULL && strcmp((*key).valor,value)==0){
			//printf("ALTURA:%d\n", (*root).height);
			return key;
		}
		else{
			return search_key(down, value);
		}
	}
	//printf("No encontrado\n");
	return NULL;
}

