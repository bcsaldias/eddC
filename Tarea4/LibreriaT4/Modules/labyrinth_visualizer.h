/*##########################################################*/
/*                Visualizador de laberinto                 */
/*                                                          */
/*                                                          */
/*                                                          */
/* Este módulo es el encargado de generar imagenes en BMP a */
/* partir de un laberinto.                                  */
/*##########################################################*/

#ifndef MODULES_LABYRINTH_VIS_T4_EDD_H
#define MODULES_LABYRINTH_VIS_T4_EDD_H

#include "../Structures/labyrinth.h"
#include "../BMP/qdbmp.h"

/*##########################################################*/
/*                        Constantes                        */
/*##########################################################*/

/* Cambia este numero si quieres cambiar el zoom de la imagen */
#define ZOOM 4
/* Demorará más en generar y pesará más si lo aumentas */

/* Indica el patron de colores a usar para la imagen. Oscuro por defecto */
#define DARK
/* Comenta esta linea si quieres usar el patron neutro */


/*##########################################################*/
/*                        Funciones                         */
/*##########################################################*/

/* Crea una imagen a partir de un laberinto */
/* Guarda la imagen en la ruta espeficiada (debe incluir el .bmp) */
/* Se puede elegir si mostrar o no los costos de las puertas */
/* show_door_size = 1 los muestra, show_door_size = 0 no los muestra */
void labyrinth_to_BMP(Labyrinth* lab, char* file_name, int show_door_size);

/* Asegurate de leer labyrinth.h y common.h */

/*##########################################################*/
/*                        Opcional                          */
/*##########################################################*/

/* Un byte, para un canal de color especifico (0~255) */
#define Brush unsigned char

/* Puedes marcar una habitacion arbitraria del laberinto usando esto */
/* Debes modificar el codigo de la funcion anterior en el espacio indicado */
/* (Revisa el codigo si quieres, busca por BEACONS) */
/* Puedes usar esto para dibujar la ruta recorrida a lo largo de un estado,*/
/* en caso de que estes teniendo problemas para debugear */
void draw_symbol(BMP* bmp, Room* room, Brush r, Brush g, Brush b);

#endif
