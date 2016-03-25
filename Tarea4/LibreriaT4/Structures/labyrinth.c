/*##########################################################*/
/* Laberinto                                                */
/*##########################################################*/

/* Representa un laberinto como una grilla de piezas */
/* No necesariamente cada celda de la grilla es una pieza */

#ifndef STRUCTURES_LABYRINTH_T4_EDD
#define STRUCTURES_LABYRINTH_T4_EDD
#define INFINITO 999999999
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "labyrinth.h"
#include "../Modules/common.h"
#include "hash.h"

/* Libera la memoria que esta usando este laberinto */
void labyrinth_destroy(Labyrinth* lab)
{
    for(int i = 0; i < lab -> width; i++)
    {
        for(int j = 0; j < lab -> height; j++)
        {
            if (lab -> grid[i][j] != NULL){
                free(lab -> grid[i][j] -> vecinas);
                free(lab -> grid[i][j]);
            }
        }
        free(lab -> grid[i]);
    }
    if(lab -> colors)
        free(lab -> colors);
    free(lab -> grid);
    free(lab -> lab_ID);
    free(lab);
}

/*##########################################################*/
/* Transformaciones                                         */
/*##########################################################*/

/* Rota la pieza en el sentido de las manecillas del reloj */
void rotate_room(Room* room)
{
    int aux       = room -> left;
    room -> left  = room -> down;
    room -> down  = room -> right;
    room -> right = room -> up;
    room -> up    = aux;
}

void copy_room(Room * new_room, Labyrinth * labyrinth, int i, int j){
            //printf("CREANDOME-");
            new_room -> x = i;
            new_room -> y = j;
            new_room -> position = 0;
            new_room -> color = labyrinth -> grid[i][j] -> color;
            new_room -> right = labyrinth -> grid[i][j] -> right;
            new_room -> up = labyrinth -> grid[i][j] -> up;
            new_room -> left = labyrinth -> grid[i][j] -> left;
            new_room -> down = labyrinth -> grid[i][j] -> down;
            new_room -> vecinas = try_malloc(sizeof(Room*) * 7);
            for (int k = 0; k < 7; k++)
            {
                new_room -> vecinas[k] = labyrinth -> grid[i][j]->vecinas[k];
            }
            new_room -> padre = NULL;
            new_room -> distancia = INFINITO;
}
/* Rota todas las piezas del color especificado */
/* Si quieres que sea mas rapido, haz una lista de las piezas por color */
Labyrinth * rotate_color(Labyrinth *lab, Labyrinth* labyrinth, Color color)
{
    lab = try_malloc(sizeof(Labyrinth));
    lab -> width = labyrinth -> width;
    lab -> height = labyrinth -> height;

    //int *NID
    lab -> lab_ID = try_malloc(sizeof(int)*7);
    for (int j = 0; j < 7; ++j)
    {
        lab -> lab_ID[j] = labyrinth -> lab_ID[j];
    }
    int position = color_number(color) -1;
    int value = (labyrinth -> lab_ID[position] + 1 ) % 4;
    lab -> lab_ID[position] = value;
    lab -> ID = hash(lab->lab_ID);
    //printf("Resultado: %d\n", lab->ID);

    lab -> grid = try_malloc(sizeof(Room**) * lab -> width);
    for(int i = 0;i < lab -> width; i++)
    {
        lab -> grid[i] = try_malloc(sizeof(Room*) * lab -> height);
        for(int j = 0; j < lab -> height; j++)
        {

            if (labyrinth->grid[i][j] != NULL){
                lab -> grid[i][j] = try_malloc(sizeof(Room));
                copy_room(lab -> grid[i][j], labyrinth, i, j);
                lab -> grid[i][j] -> ID = lab -> ID;
            }else{
                lab -> grid[i][j] = NULL;
            }
        }
    }

    //lab -> lab_ID = NID;
    lab -> color_count = labyrinth -> color_count;
    lab -> colors = try_malloc(sizeof(Color) * labyrinth -> color_count);
    for (int i = 0; i < lab -> color_count; ++i)
    {
        lab -> colors[i] = labyrinth -> colors[i];
    }

    lab -> start = lab -> grid[labyrinth->start->x][labyrinth->start->y];
    lab -> finish = lab -> grid[labyrinth->finish->x][labyrinth->finish->y];
    
    for(int i = 0; i < lab -> width; i++)
    {
        for(int j = 0; j < lab -> height; j++)
        {
            Room* room = lab -> grid[i][j];
            if(room && room -> color == color)
            {
                rotate_room(lab -> grid[i][j]);
            }
        }
    }
    

    return lab;
}

/*##########################################################*/
/* I/O                                                      */
/*##########################################################*/

void file_error(FILE* file)
{
    fclose(file);
    fprintf(stderr, "Error leyendo laberinto\n");
    exit(5);
}


/* Lee un laberinto guardado en un archivo de texto plano */
Labyrinth* labyrinth_from_file(const char* filename)
{
    FILE* file = fopen(filename, "r");
    if(file == NULL)
    {
        fprintf(stderr, "Error leyendo laberinto\n");
        exit(5);
    }

    Labyrinth* lab = try_malloc(sizeof(Labyrinth));

    char buffer[8];

    // Me pareció que ponerle un if a cada uno quedaba muy muy sucio
    // fscanf(...) ?: file_error(file) es equivalente a
    // if(!fscanf(...)) file_error(file)

    fscanf(file,"%s", buffer)                              ?: file_error(file);
    if(strcmp(buffer,"WIDTH"))                                file_error(file);
    fscanf(file,"%lu",&lab -> width)                       ?: file_error(file);
    fscanf(file,"%s", buffer)                              ?: file_error(file);
    if(strcmp(buffer,"HEIGHT"))                               file_error(file);
    fscanf(file,"%lu",&lab -> height)                      ?: file_error(file);

    lab -> grid = try_malloc(sizeof(Room**) * lab -> width);
    for(int i = 0;i < lab -> width; i++)
    {
        lab -> grid[i] = try_malloc(sizeof(Room*) * lab -> height);
        for(int j = 0; j < lab -> height; j++)
        {
            lab -> grid[i][j] = NULL;
        }
    }

    fscanf(file,"%s", buffer)                              ?: file_error(file);
    if(strcmp(buffer,"COLORS"))                               file_error(file);
    fscanf(file,"%d",&lab -> color_count)                  ?: file_error(file);

    lab -> colors = try_malloc(sizeof(Color) * lab -> color_count);
    lab -> lab_ID = try_malloc(sizeof(int) * 7); // hay máximo  colores

    for (int i = 0; i < 7; i++)
    {
        lab -> lab_ID[i] = 0; //todos se inicializan en el estado inicial
    }
    lab-> ID = 0;

    for(int i = 0; i < lab -> color_count; i++)
    {
        fscanf(file,"%u",&lab -> colors[i])                ?: file_error(file);
    }

    int x = -1;
    int y = -1;

    while(1)
    {
        fscanf(file,"%s", buffer)                          ?: file_error(file);
        if(!strcmp(buffer,"ROOM"))
        {
            fscanf(file,"%d",&x)                           ?: file_error(file);
            fscanf(file,"%d",&y)                           ?: file_error(file);

            lab -> grid[x][y] = try_malloc(sizeof(Room));
            lab -> grid[x][y] -> x = x;
            lab -> grid[x][y] -> y = y;

            fscanf(file,"%u",&lab -> grid[x][y] -> color)  ?: file_error(file);
            fscanf(file,"%d",&lab -> grid[x][y] -> right)  ?: file_error(file);
            fscanf(file,"%d",&lab -> grid[x][y] -> up)     ?: file_error(file);
            fscanf(file,"%d",&lab -> grid[x][y] -> left)   ?: file_error(file);
            fscanf(file,"%d",&lab -> grid[x][y] -> down)   ?: file_error(file);

            lab -> grid[x][y] -> vecinas = try_malloc(sizeof(Room*) * 7);
            for (int i = 0; i < 7; i++)
            {
                lab -> grid[x][y] -> vecinas[i] = NULL;
            }
            lab -> grid[x][y] -> padre = NULL;
            lab -> grid[x][y] -> distancia = INFINITO;
            lab -> grid[x][y] -> ID = lab -> ID;
            lab -> grid[x][y] -> position = 0;


        }
        else if(!strcmp(buffer,"START"))
        {
            break;
        }
        else                                                  file_error(file);
    }


    fscanf(file,"%d",&x)                                   ?: file_error(file);
    fscanf(file,"%d",&y)                                   ?: file_error(file);

    lab -> start = lab -> grid[x][y];

    fscanf(file,"%s", buffer)                              ?: file_error(file);
    if(strcmp(buffer,"FINISH"))                               file_error(file);

    fscanf(file,"%d",&x)                                   ?: file_error(file);
    fscanf(file,"%d",&y)                                   ?: file_error(file);

    lab -> finish = lab -> grid[x][y];

    fclose(file);

    return lab;
}

/* Imprime el laberinto como un archivo de texto plano */
void labyrinth_to_file(Labyrinth* lab, const char* filename)
{
    FILE* file = fopen(filename, "w");
    // Imprimir dimensiones
    fprintf(file,"WIDTH  %lu\n",lab -> width);
    fprintf(file,"HEIGHT %lu\n",lab -> height);

    // Imprimir colores
    fprintf(file,"COLORS %d ", lab -> color_count);

    for(int c = 0; c < lab -> color_count; c++)
    {
        fprintf(file, "%u ", lab -> colors[c]);
    }
    fprintf(file,"\n");

    // Imprimir pieza por pieza
    for(int i = 0; i < lab -> width; i++)
    {
        for(int j = 0; j < lab -> height; j++)
        {
            Room* room = lab -> grid[i][j];
            if(room)
            {
                fprintf(file,"ROOM   ");
                fprintf(file,"%d ", room -> x);
                fprintf(file,"%d ", room -> y);
                fprintf(file,"%d ", room -> color);
                fprintf(file,"%d ", room -> right);
                fprintf(file,"%d ", room -> up);
                fprintf(file,"%d ", room -> left);
                fprintf(file,"%d\n", room -> down);
            }
        }
    }
    fprintf(file, "START  %d %d\n",lab -> start -> x, lab -> start -> y);
    fprintf(file, "FINISH %d %d\n",lab -> finish -> x, lab -> finish -> y);

    fclose(file);
}



Labyrinth * new_labe(Labyrinth *lab, Labyrinth* labyrinth, int menor_x, int mayor_x, int menor_y, int mayor_y)
{

    int ancho = mayor_x - menor_x +1;
    int alto = mayor_y - menor_y +1;

    lab = try_malloc(sizeof(Labyrinth));
    lab -> width = ancho;
    lab -> height = alto;

    //int *NID
    lab -> lab_ID = try_malloc(sizeof(int)*7);
    for (int j = 0; j < 7; ++j)
    {
        lab -> lab_ID[j] = labyrinth -> lab_ID[j];
    }
    lab -> ID = hash(lab->lab_ID);
    //printf("Resultado: %d\n", lab->ID);

    lab -> grid = try_malloc(sizeof(Room**) * lab -> width);
    for(int i = 0;i < lab -> width; i++)
    {
        lab -> grid[i] = try_malloc(sizeof(Room*) * lab -> height);
        for(int j = 0; j < lab -> height; j++)
        {

            if (labyrinth->grid[i+menor_x][j+menor_y] != NULL){
                lab -> grid[i][j] = try_malloc(sizeof(Room));
                copy_room(lab -> grid[i][j], labyrinth, i+menor_x, j+menor_y);
                lab -> grid[i][j]-> x = i;
                lab -> grid[i][j]-> y = j;
                lab -> grid[i][j] -> ID = lab -> ID;
                
            }else{
                lab -> grid[i][j] = NULL;
            }
        }
    }

    lab -> start = lab -> grid[labyrinth->start->x - menor_x][labyrinth->start->y - menor_y];
    lab -> finish = lab -> grid[labyrinth->finish->x - menor_x][labyrinth->finish->y - menor_y];



    int * COL = malloc(sizeof(int)*7);
    for (int i = 0; i < 7; ++i)
    {
        COL[i] = 0;
    }


    for (int i = 0; i < lab->width; ++i)
    {
        for (int j = 0; j < lab->height; ++j)
        {
            Room * r = lab->grid[i][j];
            if (r){
                if (r->color < 8 && r->color > 0)
                    COL[r->color-1] = 1;
            }
        }
    }

    int cuantos_colores = 0;
    for (int i = 0; i < 7; ++i)
    {
        if(COL[i])
            cuantos_colores +=1;
    }
    lab->color_count = cuantos_colores;

    //int *lista = malloc(sizeof(int)*cuantos_colores);
    lab -> colors = try_malloc(sizeof(Color) * cuantos_colores);
    for (int i = 0; i < cuantos_colores; ++i)
    {
        lab -> colors[i] = 0;
    }
    int prim = 0;
    for (int i = 0; i < 7; ++i)
    {
        if(COL[i]){
            lab -> colors[prim] = i+1;
            prim += 1;
        }
    }
    //lab->colors = lista;

    free(COL);
    //return NULL;

    return lab;

}


#endif
