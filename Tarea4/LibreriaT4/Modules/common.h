/*##########################################################*/
/*                       Modulo común                       */
/*                                                          */
/*                                                          */
/*                                                          */
/* Este módulo contiene las multiples definiciones usadas a */
/* lo largo de todos los modulos de la tarea.               */
/*##########################################################*/

#ifndef EDD_T4_COMMON_H
#define EDD_T4_COMMON_H
#include <stdlib.h>
#include <stdio.h>

/*##########################################################*/
/*                          Tipos                           */
/*##########################################################*/

/* Define los distintos tipos de celda que hay en el laberinto */
enum type {neutral,red,orange,yellow,green,cyan,blue,purple,start,finish};
/* Se diferencian principalmente por su color */
typedef enum type Color;

/*##########################################################*/
/*                        Funciones                         */
/*##########################################################*/

/* Calcula el costo de pasar por una puerta, en caso que esta exista */
/* dado el ancho de las dos puertas que la forman y el tamaño de la multitud */
int compute_passage_cost(int door_width_1, int door_width_2, int crowd_size)
{
    /* Si alguno de los anchos es 0 corta el programa */
    if(door_width_1 == 0 || door_width_2 == 0)
    {
        return -1;
        fprintf(stderr, "%s\n", "ESA PUERTA NO EXISTE");
        exit(10);
    }

    /* El ancho de la puerta esta dado por el minimo entre ambos anchos */
    int door_width = door_width_1 < door_width_2 ? door_width_1 : door_width_2;
    /* En cuantas pasadas puede pasar el grupo de n personas si pasan x */
    /* en cada pasada */
    /* x = door_size */
    /* n = crowd_size */
    int cost = crowd_size / door_width;
    if(crowd_size % door_width)
    {
        cost++;
    }

    return cost;
}

/* Entrega el nombre de la trancision de rotar el color dado */
char* transition_name(Color color)
{
    switch(color)
    {
        case red:       return "ROTATE RED";
        case orange:    return "ROTATE ORANGE";
        case yellow:    return "ROTATE YELLOW";
        case green:     return "ROTATE GREEN";
        case cyan:      return "ROTATE CYAN";
        case blue:      return "ROTATE BLUE";
        case purple:    return "ROTATE PURPLE";
        default:        return "NO TRANSITION";
    }
}

/* Para debugeo */
/* Entrega un color como texto para poder imprimirlo */
char* color_name(Color color)
{
    switch(color)
    {
        case red:       return "RED";
        case orange:    return "ORANGE";
        case green:     return "GREEN";
        case blue:      return "BLUE";
        case purple:    return "PURPLE";
        case yellow:    return "YELLOW";
        case cyan:      return "CYAN";
        case neutral:   return "NEUTRAL";
        default:        return "NO COLOR";
    }
}

/* Entrega un numero como referencia a un color para poder imprimirlo */
int color_number(Color color)
{
    switch(color)
    { // enum type {neutral,red,orange,yellow,green,cyan,blue,purple,start,finish};
        case red:       return 1;
        case orange:    return 2;
        case yellow:    return 3;
        case green:     return 4;
        case cyan:      return 5;
        case blue:      return 6;
        case purple:    return 7;
        case neutral:   return 0; //no pueden ser rotadas
        default:        return "NO COLOR";
    }
}


/* Revisa si el malloc falló, de ser así da el aviso y corta la ejecucion */
/* Puedes liberar tu memoria aqui si quieres */
/* Ten cuidado con modificarlo ya que la libreria usa esta funcion */
void* try_malloc(size_t size)
{
    void* pointer = malloc(size);
    if(!pointer) //If the pointer is NULL
    {
        fprintf(stderr, "Failed to allocate memory\n");
        exit(2);
    }
    return pointer;
}

/* Asegurate de leer labyrinth.h y labyrinth_visualizer.h :D */

#endif
