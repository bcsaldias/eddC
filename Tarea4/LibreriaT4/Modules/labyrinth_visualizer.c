#ifndef MODULES_LABYRINTH_VIS_T4_EDD
#define MODULES_LABYRINTH_VIS_T4_EDD

#include "../BMP/qdbmp.c"
#include "../BMP/qdbmp.h"

#include "labyrinth_visualizer.h"
#include "common.h"

//Un byte, para un canal de color especifico.
#define Brush unsigned char

#ifdef DARK
    #define BG 0
#else
    #define BG 127
#endif

//Tamaño de la pieza. Debe ser un numero impar.
#define ROOM_SIZE 15
//Grosor de las paredes
#define WALL_WIDTH 2

/* Asigna el color correspondiente a (r,g,b) segun el enum dado */
void dip_RGB_Color(Color color, Brush* r, Brush* g, Brush* b)
{
    switch(color)
    {
        #ifdef DARK
            case red:       *r = 255;   *g = 0;     *b = 0;     break;
            case orange:    *r = 252;   *g = 166;   *b = 0;     break;
            case yellow:    *r = 255;   *g = 255;   *b = 0;     break;
            case green:     *r = 0;     *g = 192;   *b = 0;     break;
            case cyan:      *r = 66;    *g = 252;   *b = 255;   break;
            case blue:      *r = 0;     *g = 60;    *b = 255;   break;
            case purple:    *r = 213;   *g = 53;    *b = 217;   break;
            default:        *r = 255;   *g = 255;   *b = 255;   break;
        #else
            case red:       *r = 221;   *g = 0;     *b = 0;     break;
            case orange:    *r = 255;   *g = 102;   *b = 0;     break;
            case yellow:    *r = 221;   *g = 221;   *b = 0;     break;
            case green:     *r = 0;     *g = 221;   *b = 0;     break;
            case cyan:      *r = 0;     *g = 221;   *b = 221;   break;
            case blue:      *r = 0;     *g = 0;     *b = 221;   break;
            case purple:    *r = 102;   *g = 0;     *b = 136;   break;
            case neutral:   *r = 0;     *g = 0;     *b = 0;     break;
            default:        *r = 255;   *g = 255;   *b = 255;   break;
        #endif
    }
}

/* Dibuja la celda que tiene como esquina superior izquierda a (x,y) */
void draw_cell(BMP* bmp, int x, int y, Room* room, int show_door_size)
{
    Brush r, g, b;
    dip_RGB_Color(room -> color, &r, &g, &b);
    //Pinta las paredes de la pieza

    for(int k = 0; k < WALL_WIDTH; k++)
    {
        int width = ROOM_SIZE - 2*k;
        int height = ROOM_SIZE - 2*k;
        for(int i = 0; i < width; i++)
        {
            for(int j = 0;j < height; j++)
            {
                if(i % (width - 1) == 0 || j % (height - 1) == 0)
                {
                    BMP_SetPixelRGB(bmp, x + k + i, y + k + j, r, g, b);
                }
                else
                {
                    BMP_SetPixelRGB(bmp, x + k + i, y + k + j, BG, BG, BG);
                }
            }
        }
        //Open holes
        if(room -> down)
        {
            int down = show_door_size ? room -> down : 3;

            int begin = x + ROOM_SIZE / 2 - down / 2;

            for(int i = begin; i < begin + down; i++)
            {
                BMP_SetPixelRGB(bmp, i, y + ROOM_SIZE - 1 - k, BG, BG, BG);
            }
        }

        if(room -> up)
        {
            int up = show_door_size ? room -> up : 3;

            int begin = x + ROOM_SIZE / 2 - up / 2;

            for(int i = begin; i < begin + up; i++)
            {
                BMP_SetPixelRGB(bmp, i, y + k, BG, BG, BG);
            }
        }
        if(room -> left)
        {
            int left = show_door_size ? room -> left : 3;

            int begin = y + ROOM_SIZE / 2 - left / 2;

            for(int j = begin; j < begin + left; j++)
            {
                BMP_SetPixelRGB(bmp, x + k, j, BG, BG, BG);
            }
        }
        if(room -> right)
        {
            int right = show_door_size ? room -> right : 3;

            int begin = y + ROOM_SIZE / 2 - right / 2;

            for(int j = begin; j < begin + right; j++)
            {
                BMP_SetPixelRGB(bmp, x + ROOM_SIZE - 1 - k, j, BG, BG, BG);
            }
        }
    }
}

void draw_symbol(BMP* bmp, Room* room, Brush r, Brush g, Brush b)
{
    int map_x = room -> x*(ROOM_SIZE + WALL_WIDTH) + WALL_WIDTH;
    int map_y = room -> y*(ROOM_SIZE + WALL_WIDTH) + WALL_WIDTH;

    int size = 7;

    int x = map_x + ROOM_SIZE / 2 - size / 2;
    int y = map_y + ROOM_SIZE / 2 - size / 2;

    for(int k = 0; k < size / 2; k++)
    {
        int width = size - 2*k;
        int height = size - 2*k;
        for(int i = 0; i < width; i++)
        {
            for(int j = 0;j < height; j++)
            {
                if(i % (width - 1) == 0 || j % (height - 1) == 0)
                {
                    int pix_x = x + k + i;
                    int pix_y = y + k + j;

                    if(k % 2 == 0)
                    {
                        BMP_SetPixelRGB(bmp, pix_x, pix_y, r, g, b);
                    }
                }
            }
        }
    }
}

void draw_start(BMP* bmp, Room* room)
{
    #ifdef DARK
        draw_symbol(bmp, room, 255, 0, 255);
    #else
        draw_symbol(bmp, room, 0, 0, 0);
    #endif
}

void draw_finish(BMP* bmp, Room* room)
{
    #ifdef DARK
        draw_symbol(bmp, room, 0, 255, 255);
    #else
        draw_symbol(bmp, room, 255, 255, 255);
    #endif
}

void labyrinth_to_BMP(Labyrinth* lab, char* file_name, int show_door_size)
{
    UINT width = (ROOM_SIZE + WALL_WIDTH) * lab -> width + WALL_WIDTH;
	UINT height = (ROOM_SIZE + WALL_WIDTH)* lab -> height + WALL_WIDTH;

    BMP* bmp = BMP_Create(width, height, 24);

    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            BMP_SetPixelRGB(bmp, i, j, BG, BG, BG);
        }
    }

    for(int i = 0; i < lab -> width; i++)
    {
        for(int j = 0; j < lab -> height; j++)
        {
            if(lab -> grid[i][j])
            {
                int x = i*(ROOM_SIZE + WALL_WIDTH) + WALL_WIDTH;
                int y = j*(ROOM_SIZE + WALL_WIDTH) + WALL_WIDTH;

                draw_cell(bmp, x, y, lab -> grid[i][j], show_door_size);
            }
        }
    }

    draw_start(bmp, lab -> start);
    draw_finish(bmp, lab -> finish);

    /* BEACONS */
    /* SI QUIERES MARCAR HABITACIONES, HAZLO AQUI */

    /*
    for(int i = 0;i < la cantidad de piezas que quieras marcar; i++)
    {
        Brush r; // Componente roja del color que quieres usar (0~255)
        Brush g; // Componente verde del color que quieres usar (0~255)
        Brush b; // Componente azul del color que quieres usar (0~255)

        draw_symbol(BMP,lista_de_punteros_a_piezas[i],r,g,b);
    }

    Tanto la lista de las piezas el largo de esa lista (o un arreglo)
    deberian estar guardados en el laberinto para leerlos desde aca.

    */
    
    /* No toques nada abajo de esto */

    width = ZOOM * width;
    height = ZOOM * height;

    if(ZOOM > 1)
    {
        BMP* bigBmp = BMP_Create(width, height, 24);
        Brush r = BG;
        Brush g = BG;
        Brush b = BG;

        for(int i = 0; i < width; i++)
        {
            for(int j = 0; j < height; j++)
            {
                BMP_GetPixelRGB(bmp, i/ZOOM, j/ZOOM, &r, &g, &b);
                BMP_SetPixelRGB(bigBmp, i, j, r, g, b);
            }
        }
        /* Esto ya no nos sirve */
        BMP_Free(bmp);

        /* Save result */
    	BMP_WriteFile(bigBmp, file_name);

    	/* Free all memory allocated for the image */
    	BMP_Free(bigBmp);
    }
    else
    {
        /* Save result */
    	BMP_WriteFile(bmp, file_name);

    	/* Free all memory allocated for the image */
    	BMP_Free(bmp);
    }
}

#endif
