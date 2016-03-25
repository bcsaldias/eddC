/*##########################################################*/
/*                        Laberinto                         */
/*                                                          */
/*                                                          */
/*                                                          */
/* Esta estructura es una representacion simple de un       */
/* laberinto, y se encarga de las operaciones asociadas     */
/*##########################################################*/
#ifndef STRUCTURES_LABYRINTH_T4_EDD_H
#define STRUCTURES_LABYRINTH_T4_EDD_H

#include "../Modules/common.h"

/*##########################################################*/
/*                          Tipos                           */
/*##########################################################*/

/* Una pieza del laberinto */
struct room;
typedef struct room Room;

/* El laberinto en si */
struct labyrinth;
typedef struct labyrinth Labyrinth;

/*##########################################################*/
/*                       Estructuras                        */
/*##########################################################*/

/* Representa una pieza del laberinto */
struct room
{
    /* Coordenada X de esta pieza en la grilla */
    int x;
    /* Coordenada Y de esta pieza en la grilla */
    int y;
    /* Color de esta pieza */
    Color color;
    /* Indica cuantos caben para salir hacia la derecha */
    int right;
    /* Indica cuantos caben para salir hacia arriba */
    int up;
    /* Indica cuantos caben para salir hacia la izquierda */
    int left;
    /* Indica cuantos caben para salir hacia abajo */
    int down;

    /* Referencia a las piezas del otro laberinto 
    
    referencia a si misma pero en el laberinto girando
    el color de la posición a la derecha
    */
    int position;
    
    struct room ** vecinas;

    Room * padre;
    int distancia;
    int ID;
};

/* Representa el laberinto como una grilla rectangular */
struct labyrinth
{
    /* La grilla, una matriz de punteros a piezas */
    struct room*** grid;
    /* Ancho de la grilla */
    size_t width;
    /* Alto de la grilla */
    size_t height;
    /* Pieza inicial del laberinto */
    struct room* start;
    /* Pieza destino del laberinto */
    struct room* finish;
    /* Cuantos colores tiene este laberinto sin considerar el negro */
    int color_count;
    /* Colores presentes en este laberinto */
    Color* colors;

    /*Id arreglo largo cuantos colores hay*/
    int *lab_ID;
    int ID;
    int R_COLOR;
};
/* Está representado asi por simplicidad, hay mucho para optimizar */

/*##########################################################*/
/*                       Operaciones                        */
/*##########################################################*/

/* Rota la pieza especificada en el sentido de las manecillas del reloj */
void rotate_room(Room* room);
/* No se preocupa de que color es. Esto es por si quieres hacer algo tu */

/* Rota todas las piezas del color especificado. Usa la funcion anterior */
Labyrinth * rotate_color(Labyrinth* lab, Labyrinth* labyrinth, Color color);
/* Si quieres que sea mas rapido, haz listas de las piezas por color */

/*##########################################################*/
/*                           I/O                            */
/*##########################################################*/


/* Lee un laberinto guardado en un archivo de texto plano */
Labyrinth* labyrinth_from_file(const char* filename);

/* Imprime el laberinto como un archivo de texto plano */
void labyrinth_to_file(Labyrinth* laberinto, const char* filename);

/*##########################################################*/
/*                     Uso de memoria                       */
/*##########################################################*/

/* Libera toda la memoria asociada a este laberinto */
void labyrinth_destroy(Labyrinth* lab);

/* Asegurate de leer common.h y labyrinth_visualizer.h */
Labyrinth * new_labe(Labyrinth *lab, Labyrinth* labyrinth, int menor_x, int mayor_x, int menor_y, int mayor_y);


#endif
