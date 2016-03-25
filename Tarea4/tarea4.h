/*##########################################################*/
/*                         Tarea 4                          */
/*                                                          */
/*                                                          */
/* Importamos todas las herramientas para resolver la tarea */
/*##########################################################*/

#ifndef CORE_EDD_T4
#define CORE_EDD_T4

/* El modulo comun */
#include "LibreriaT4/Modules/common.h"
/* La estructura para laberintos */
#include "LibreriaT4/Structures/labyrinth.c"
/* Lista para almacenar laberitons con sus distintos estados */
#include "LibreriaT4/Structures/list.c"
#include "LibreriaT4/Structures/heap.c"
#include "LibreriaT4/Structures/hash.h"
/* La herramienta para visualizar los laberintos*/
#include "LibreriaT4/Modules/labyrinth_visualizer.c"
/*##########################################################*/
/* Asegurate de leer los .h de esos 3 archivos              */
/*##########################################################*/
void create_network(Array * Estados, Labyrinth * lab, Color color);


/*Guardo el cluster encontrado como un nuevo mapa y busco el camino con ese*/
int ** encontrar_cluster(Labyrinth *lab); 

#endif
