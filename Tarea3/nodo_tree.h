
//Nodo del arbol
typedef struct NodoJ_tree{
	int height;
	struct NodoJ_tree* child;
	struct NodoJ_tree* next;
	struct NodoJ_tree* father;
	struct NodoJ* keys;
}NodoJ_tree;


int insert_key(NodoJ_tree* nt, char* value, char* value_retorno);

void init_nt(NodoJ_tree* nt, int height){
	(*nt).height=height;
	(*nt).child = NULL;
	(*nt).next = NULL;
	(*nt).father=NULL;
	(*nt).keys = NULL;
}

NodoJ_tree* new_nt(int height){
	NodoJ_tree* new_nt = malloc(sizeof(NodoJ_tree));
	init_nt(new_nt, height);
	return new_nt;
}

//Una hoja no tiene hijos y solo tiene una llave
NodoJ_tree* new_leaf(char* value, char* value_retorno){
	NodoJ_tree* new_leaf = new_nt(0);
	insert_key(new_leaf, value, value_retorno);
	return new_leaf;
}

//Insertar una llave a un nodo del arbol. Se inserta la llave a la lista ligada de llaves que posee ese nodo, ordenadas segun valor.
int insert_key(NodoJ_tree* nt, char* value, char* value_retorno){
	NodoJ* new_key = new_nodo(value, value_retorno);
	return insert_nodo(&((*nt).keys),new_key);
}

//Inserta un nodo hijo a un nodo especifico del skip tree. Lo inserta en la lista ligada de hijos (nodos del arbol), en la posicion especificada.
void insert_child(NodoJ_tree* nt, NodoJ_tree* child, int index){
	(*child).father = nt;
	if((*nt).child==NULL){
		(*nt).child= child;
	}
	else{
		NodoJ_tree* actual = (*nt).child;
		int i;
		for(i=0; i<index;i++){
			actual = (*actual).next;
		}
		NodoJ_tree* next = (*actual).next;		
		(*child).next = next;
		(*actual).next = child;
	}
}


void nt_to_string(NodoJ_tree* nt){
	if(nt!=NULL){
		printf("Nivel: %d\nLlaves:", (*nt).height);
		print_nodos((*nt).keys);
		if((*nt).child!=NULL){
			NodoJ_tree* actual = (*nt).child;
			while(actual!=NULL){
				nt_to_string(actual);
				actual = (*actual).next;
			}
		}
	}
	else{
		printf("NodoJ de arbol nulo\n");
	}
}

//Simula el metodo attach del skip tree. Le anade un nodo hijo al nodo especificado.
//Este hijo es una hoja en el nivel 0, con una sola llave.
void nt_attach_value(NodoJ_tree* nt, char* value, char* value_retorno){
	NodoJ_tree* leaf = new_leaf(value, value_retorno);
	int index = insert_key(nt, value, value_retorno);
	insert_child(nt, leaf, index);
}

