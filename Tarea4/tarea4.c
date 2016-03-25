#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "tarea4.h"

int reduce(Room *u, Room *v, int costo){
	if (v->distancia >= u->distancia + costo){
		v->distancia = u->distancia + costo;
		v->padre = u;
        return 1;
	}
    return 0;
}



int Dijkstra(Array * Estados, Labyrinth* lab, Room * r, int crowd){

    r -> distancia = 0;


	Heap *heap = heap_init(lab->width*lab->height*pow (4, 7));
    for (int i = 0; i < pow (4, 7); ++i)
    {
    	Labyrinth* un_lab = Estados->content[i]->first->value;

    	if (un_lab){

	    	for (int k = 0; k < un_lab->width; ++k)
	    	{
		    	for (int j = 0; j < un_lab->height; ++j)
		    	{
		            Room* room = un_lab -> grid[k][j];
		            if(room)
		            {
		            	basicInsertion(heap, un_lab -> grid[k][j]);
		            }
		    	}
	    	}

    	}
    }
    buildHeap(heap);

	while (heap -> size > 0){
		Room * u = xMin(heap);


		if (u != NULL){

            Labyrinth * e = Estados->content[u->ID]-> first -> value;

            for (int k = 0; k < 7; k++)
		    {
		    	if (u->vecinas[k]){
			        if (reduce( u, u->vecinas[k], 1)){
                        decrementKey(heap, u->vecinas[k]->position);
                    }
		    	}
		    }

            int costo;
            Room * otra; 
            
            // arriba
            if (u->y-1 >=0 && e->grid[u->x][u->y-1]){
                otra = e->grid[u->x][u->y-1];
                costo = compute_passage_cost(u->up,otra->down, crowd);
                if(costo > -1){
                    if(reduce( u, otra, costo)){
                        decrementKey(heap, otra->position);
                    }
                }
            }

            // izquierda
            if (u->x-1 >=0 && e->grid[u->x-1][u->y]){
                otra = e->grid[u->x-1][u->y];
                costo = compute_passage_cost(u->left,otra->right, crowd);
                if(costo > -1){
                    if(reduce( u, otra, costo)){
                        decrementKey(heap, otra->position);
                    }
                }
            }
            // derecha
            if (u->x+1 < e->width && e->grid[u->x+1][u->y]){
                otra = e->grid[u->x+1][u->y];
                costo = compute_passage_cost(u->right,otra->left, crowd);
                if(costo > -1){
                    if(reduce( u, otra, costo)){
                        decrementKey(heap, otra->position);
                    }
                }
            }
            // abajo
            if (u->y+1 < e->height && e->grid[u->x][u->y+1]){
                otra = e->grid[u->x][u->y+1];
        
                costo = compute_passage_cost(u->down,otra->up, crowd);

                if(costo > -1){
                    if(reduce( u, otra, costo)){
                        decrementKey(heap, otra->position);
                    }
                }
            }


    	}
	}

    int RESULTADO = 99999999;

    Room * last_room;
    Labyrinth*  _un_lab;
    for (int i = 0; i < pow (4, 7); ++i)
    {
        Labyrinth* un_lab = Estados->content[i]->first->value;

        if (un_lab){
        	_un_lab = un_lab;
            if (un_lab->finish->distancia< RESULTADO){
                    RESULTADO = un_lab->finish->distancia;
                    last_room = un_lab->finish;
                }
        }

    }

    int largo_camino = 0;
    Room * next = last_room;

    while(next != NULL){
    	largo_camino += 1;

    	/* (int i = 0; i <7; ++i)
    	{
    		printf("%d-", Estados->content[next->ID]->first->value->lab_ID[i]);
    	}
    	printf(" --- %d %d %d,",next->ID, next->x, next->y);
    	printf("\n");*/

    	next = next->padre;

    }

    Room *SOLUCION[largo_camino];

    int puntero = largo_camino -1;
    while(last_room != NULL){
    	//printf("puntero %d\n", puntero);

    	SOLUCION[puntero] = last_room;
    	last_room = last_room->padre;
    	puntero-=1;
    }

    for (int i = 0; i < largo_camino - 1; ++i)
    {
    	Room * hija = SOLUCION[i];
    	Room * padre = SOLUCION[i+1];
    
    	if (padre->y > hija->y){
    		printf("GO DOWN");	
    	}else if (padre->y < hija->y){
    		printf("GO UP");
    	}else if (padre->x > hija->x){
    		printf("GO RIGHT");
    	}else if (padre->x < hija->x){
    		printf("GO LEFT");
    	}else{
    		int * ID_padre = Estados->content[padre->ID]->first->value->lab_ID;
    		int * ID_hijo = Estados->content[hija->ID]->first->value->lab_ID;
    		for (int j = 0; j < 7; j++)
    		{
    			if (ID_padre[j] != ID_hijo[j]){
    				printf(transition_name(j+1));
    			}
    		}
    	}
    	printf("\n");
    }

    //printf("\n");


    //printf("%d\t%d\n", crowd, RESULTADO);
    heap_destroy(heap);
    return RESULTADO;
}

void create_network(Array * Estados, Labyrinth * lab, Color color){

    if(lab -> color_count)
    {

        Labyrinth * clon = rotate_color(clon, lab, color);
        clon -> R_COLOR = color;
        insert_array(Estados, clon);


        for(int i = 0; i < clon -> color_count;i++)
        {
       		int NID[7];
		    for (int j = 0; j < 7; ++j)
		    {
		    	NID[j] = clon -> lab_ID[j];
		    }
		    int position = color_number(clon -> colors[i]) -1;

			int value = (clon -> lab_ID[position] + 1 ) % 4;
		    NID[position] = value;

            if (Estados->content[hash(NID)]->size == 0){
            	create_network(Estados, clon, clon -> colors[i]);
            }


            for(int k = 0;k < clon -> width; k++)
            {
                for(int j = 0; j < clon -> height; j++)
                {

                    Room* room = clon -> grid[k][j];
                    if(room && room -> color != lab -> colors[i])
                    {
                    
                        Labyrinth * b = Estados->content[hash(NID)]->first->value;
                        clon->grid[k][j] -> vecinas[clon->colors[i]-1] = b-> grid[k][j];
                    }
                }
            }

        }
    }
}
int main2(const char* filename, int crowd);
int main(int argc, char const *argv[]){

    if(argc != 3)
    {
        printf("Usage: %s <LabyrinthFileName> <AmountOfPeople>\n",argv[0]);
        return 1;
    }

    /* Ruta al archivo de laberinto */
    const char* filename = argv[1];
    /* Tamaño de la multitud */
    int crowd = atoi(argv[2]);


    main2(filename, crowd);
    
}

int main2(const char* filename, int crowd)
{


    Labyrinth* lab = labyrinth_from_file(filename);

    int ** cluster = encontrar_cluster(lab);

    int menor_x = lab->width;
    int menor_y = lab->height;
    int mayor_x = 0;
    int mayor_y = 0;

    for (int i = 0; i < lab->width; ++i)
    {
        for (int j = 0; j < lab->height; ++j)
        {
            if (cluster[i][j] == 0){
            }else{
                if(i < menor_x)
                    menor_x = i;
                if(j < menor_y)
                    menor_y = j;
            }
        }
    }


    for (int i = 0; i < lab->width; ++i)
    {
        for (int j = 0; j < lab->height; ++j)
        {
            Room * r = lab->grid[i][j];
            if(r && cluster[i][j]){
                if(r->x > mayor_x)
                    mayor_x = r->x;
                if(r->y > mayor_y)
                    mayor_y = r->y;
            }
        }
    }
    
    Labyrinth *new_one = new_labe(new_one, lab, menor_x, mayor_x, menor_y, mayor_y);

    
    for (int i = 0; i < lab->width; ++i)
    {
    	free(cluster[i]); 
    }
    free(cluster); 


    labyrinth_destroy(lab);



    lab = new_one;

    //printf("Start %d %d\n", lab->start->x, lab->start->y);
    //printf("End %d %d\n", lab->finish->x, lab->finish->y);

    int max = pow (4, 7);
	Array* Estados = array_init(max);


    for(int i = 0; i < lab -> color_count;i++)
    {

            int NID[7];
            for (int j = 0; j < 7; ++j)
            {
                NID[j] = lab -> lab_ID[j];
            }
            int position = color_number(lab -> colors[i]) -1;

            int value = (lab -> lab_ID[position] + 1 ) % 4;
            NID[position] = value;

            if (Estados->content[hash(NID)]->size == 0){
                create_network(Estados, lab, lab -> colors[i]);
            }

    }
    if (lab -> color_count == 0)
    {
        insert_array(Estados, lab);
    }



    int current_x = lab-> start-> x;
    int current_y = lab-> start-> y;
    struct room* current_room;
    current_room = lab->grid[ current_x][ current_y];

    int response = Dijkstra(Estados, lab, Estados->content[0]->first->value->start, crowd);



    if (lab -> color_count != 0)
    {
    	labyrinth_destroy(lab);
    }
    destroy_array(Estados);

    return response;
}

void buscar_vecinas(Labyrinth * lab, Room * r, int ** visitadas){

    if (r && visitadas[r->x][r->y] == 0){
        visitadas[r->x][r->y] = 1;
        if (r->x+1 < lab->width)
            buscar_vecinas(lab, lab->grid[r->x+1][r->y], visitadas);
        if (r->x-1 >= 0)
            buscar_vecinas(lab, lab->grid[r->x-1][r->y], visitadas);
        if (r->y+1 < lab->height)    
            buscar_vecinas(lab, lab->grid[r->x][r->y+1], visitadas);
        if (r->y-1 >= 0)
            buscar_vecinas(lab, lab->grid[r->x][r->y-1], visitadas);
    }

}

int ** encontrar_cluster(Labyrinth *lab){
    int **visitadas = malloc(sizeof(int)*lab->width);
    for (int i = 0; i < lab->width; ++i)
    {
        visitadas[i] = malloc(sizeof(int)*lab->height);
    }
    for (int i = 0; i < lab->width; ++i)
    {
        for (int j = 0; j < lab->height; ++j)
        {
            visitadas[i][j] = 0;
        }
    }
    Room * r = lab->start;
    buscar_vecinas(lab, r, visitadas);



    return visitadas;
}