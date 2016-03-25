#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
El grafo se representa por Nodos que se conectan entre si.
*/
typedef struct Nodo{
	int id;
	int hijos_count;
	int *hijos_ids;
	int color; 
	int distancia;
}Nodo;


Nodo* init_Nodo(int id, int cant_hijos, int *hijos_ids){
	Nodo *nodo = malloc(sizeof(Nodo));
	nodo -> id = id;
	nodo -> hijos_count = cant_hijos;
	nodo -> hijos_ids = malloc(cant_hijos * sizeof(int));
	memcpy(nodo -> hijos_ids, hijos_ids, cant_hijos * sizeof(int));

	return nodo;
}

void destroy_nodo(Nodo *nodo){

	if (nodo != NULL){
		free(nodo->hijos_ids);
		free(nodo);
	}	
}





/*








Estructura para guardar Nodos
*/
typedef struct Array
{
    int size; 
    Nodo **content;

}Array;

Array *array_init(int len)
{
    Array *array = malloc(sizeof(*array));
    array -> size = len;
    array -> content = malloc(sizeof(Nodo) * array->size);
    return array;
}

void destroy_array(Array *array){
	int i;
	for (i = 0; i < array->size; ++i)
	{
		destroy_nodo(array->content[i]);
	}
	free(array->content);
	free(array);
}






/* 

Aquí se copia la estructura creada por el ayudante Pierre-Victor, gracias.

---------------- Memory manipulation for data structure ---------------- */

typedef struct Element
{
    int value; 
    struct Element *previous;
    struct Element *next;

}Element;

typedef struct LinkedList
{
    Element *first;
    Element *last;
    int size;
}LinkedList;

LinkedList *list_init()
{
    LinkedList *list = malloc(sizeof(*list));
    Element *element = malloc(sizeof(*element));

    if (list == NULL || element == NULL)
    {
        exit(EXIT_FAILURE);
    }

    element->value = 0;
    element->next = NULL;
    element->previous = NULL;

    list->first = element;
    list->last = element;
    list->size = 0;

    return list;
}

void list_destroy(LinkedList *list)
{

    Element *element = list->first;
    while (element) {
            Element *next = element->next;
            free(element);
            element = next;
    }
    free(list);
}

/* ---------------- Manipulation of the data structure ---------------- */

void list_add(LinkedList *list, int iValueToAdd)
{

    Element *element = malloc(sizeof(*element));

    if (element == NULL)
    {
        exit(EXIT_FAILURE);
    }

    element->value = iValueToAdd;
    element->next = NULL;

    if (list->size == 0)
    {
        free(list->first);
        element->previous = NULL;
        list->first = element;

    }
    else
    {
        element->previous = list->last;
        list->last->next = element;
    }

    list->last = element;
    list->size += 1;
}

void list_del(LinkedList *list, int iValueToDel)
{

    Element *elementCurrent, *elementPrevious;

    if (list->size == 0)
    {
        printf("ERROR. List is empty, nothing to delete\n");
    }
    else if (list->size == 1)
    {
        elementCurrent = list->first;
        if (elementCurrent->value == iValueToDel)
        {
            free(elementCurrent);
            list->first = NULL;
            list->last = NULL;
            list->size = 0;
        }
        else
        {
            printf("ERROR. Element not in list, nothing to delete\n");
        }
    }
    else
    {
        elementPrevious = list->first;
        elementCurrent = elementPrevious->next;

        if (elementPrevious->value == iValueToDel)
        {
            list->first = elementCurrent;
            elementCurrent->previous = NULL;
            list->size -= 1;
            free(elementPrevious);
        }
        else
        {
            int iKeepGoing = 1;
            while (elementCurrent != NULL && iKeepGoing == 1)
            {
                if (elementCurrent->value == iValueToDel)
                {
                    iKeepGoing = 0;
                    elementPrevious->next = elementCurrent->next;
                    list->size -= 1;
                    if (elementCurrent->next == NULL)
                    {
                        list->last = elementPrevious;
                    }
                    else
                    {
                        elementCurrent->next->previous = elementPrevious;
                    }
                    free(elementCurrent);
                }
                else
                {
                    elementPrevious = elementCurrent;
                    elementCurrent = elementCurrent->next;
                }
            }
            if (iKeepGoing == 1)
            {
                printf("ERROR. Element not found.\n");
            }
        }
    }
}

int list_get(LinkedList *list, int iPosition)
{

    Element *elementCurrent;

    if (iPosition > list->size - 1)
    {
        printf("ERROR. The list only contains %d elements.\n", list->size);
        return -1;
    }
    else
    {
        int i;
        elementCurrent = list->first;
        for (i = 0; i < iPosition; i++)
        {
            elementCurrent = elementCurrent->next;
        }
        return elementCurrent->value;
    }
}


/*


Para un mejor entendimiento del manejo de una cola se utiliza
una fachada, es así como se crea la estructura Queue.
*/

typedef struct Queue
{
	LinkedList *list;

}Queue;


Queue *cola_init()
{
    Queue *cola = malloc(sizeof(*cola));
    cola->list = list_init();

    return cola;
}

void enqueue(Queue *cola, int value){

	list_add(cola->list, value);
}

int dequeue(Queue * cola){

	int to = list_get(cola->list,0);
	list_del(cola->list, to);
	return to;

}

void destroy_queue(Queue *queue){
	list_destroy(queue->list);
	free(queue);
}



/*
Básicamente buscar aplica BFS, es decir,
una búsqueda por amplitud.
*/
int buscar(int origen, int destino, int nodos, Array *el_array){

	if (origen == destino){
		printf("%i %i %i\n", origen, destino, -1);
		return 0;
	}
	Queue *cola = cola_init();
	int i ;
	for (i = 0; i < nodos; ++i)
	{
		if (i != origen){
			el_array->content[i]->color = 0; 
			el_array->content[i]->distancia = -1;
		}
	}
	el_array->content[origen]->color = 1;
	el_array->content[origen]->distancia = 0;
	enqueue(cola, origen);

	while (cola->list->size >0){
		int u = dequeue(cola);
		Nodo *nodo = el_array->content[u];
		for (i = 0; i < nodo->hijos_count; ++i)
		{
			int id = nodo->hijos_ids[i];
			Nodo *otro = el_array->content[id];
			if (otro->color == 0){
				otro->color = 1;
				otro->distancia = nodo->distancia+1;
				enqueue(cola, id);
			}
		}
	}
	if (el_array->content[destino]->distancia > 0){
		el_array->content[destino]->distancia--;
	}
	printf("%d %d %d\n", origen, destino, el_array->content[destino]->distancia);


	/*
	Liberación de memoria usada
	*/
	destroy_queue(cola);
	return 0;

}

/*



MAIN


*/
int main(int argc, char const *argv[])
{
	/*
	En primer lugar se guarda en la variable "casos" la 
	cantidad de casos que se quiere evaluar
	*/
	int casos;
	scanf("%d", &casos);
	
	int nodos;
	Array *el_array;

	/*
	Lo que viene a continuación se realiza para cada caso
	*/

	int caso;
	for (caso = 0; caso < casos; caso++)
	{


		/*
		Se lee la cantidad de nodos del grafo del caso
		actual
		*/
		scanf("%d", &nodos);

		/*
		Se crea un array donde se guardarán los nodos
		para posteriormente reconocerlos por su índice/id
		*/
		el_array = array_init(nodos);


		/*
		El siguiente for es para crear los nodos
		y guardarlos en el arreglo
		*/
		int i ;
		for(i = 0; i < nodos; ++i){

			/*
			Se escanea el id del nodo i, este id
			es igual a i por convención.
			*/
			int id;
			scanf("%d", &id);

			/*
			Se escanea la cantidad de hijos del nodo i
			*/
			int cant_hijos;
			scanf("%d", &cant_hijos);

			/*
			Cada nodo tendrá un arreglo con los id's de
			los nodos a los que puede llegar
			*/
			int hijos_ids[cant_hijos];

			int j;
			for (j = 0; j < cant_hijos; ++j)
			{
				scanf("%d", &hijos_ids[j]);
			}

			/*
			Se guarda el nodo correspondiente en la 
			posición correspondiente del arreglo
			*/
			el_array->content[i] = init_Nodo(id, cant_hijos, hijos_ids);


			/*
			Se lee la siguiente línea/ el siguiente nodo
			*/

		};

		/*
		Se lee x1 y x2 para proceder a la búsqueda de
		intermediarios
		*/
		int from;
		scanf("%d", &from);
		int to;
		scanf("%d", &to);
		buscar(from, to, nodos, el_array);
		if (caso != casos -1){
			printf("\n");
		}
		destroy_array(el_array);


	}
	return 0;
}