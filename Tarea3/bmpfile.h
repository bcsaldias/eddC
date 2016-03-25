#ifndef bmpp
#define bmpp

#include <stdio.h>
 
/* structs  */
typedef struct
{
unsigned char fileMarker1; /* 'B' */
unsigned char fileMarker2; /* 'M' */
unsigned int bfSize;
unsigned short unused1;
unsigned short unused2;
unsigned int imageDataOffset; /* Offset to the start of image data */
}FILEHEADER;

typedef struct
   {     unsigned int   biSize;      
         int            width;            /* Width of the image */
         int            height;           /* Height of the image */
         unsigned short planes;        
         unsigned short bitPix;        
         unsigned int   biCompression;        
         unsigned int   biSizeImage;       
         int            biXPelsPerMeter; 
         int            biYPelsPerMeter;       
         unsigned int   biClrUsed;        
         unsigned int   biClrImportant;    
 }INFOHEADER;
 
 
typedef struct
 {      unsigned char  r;         /* Red value */
        unsigned char  g;         /* Green value */
        unsigned char  b;         /* Blue value */
 }Color;

char *concat_color(Color color){
    char *concatenate = malloc(20);
    snprintf(concatenate,20,"%d %d %d", color.r, color.g, color.b);

    return concatenate;
}


/*
Structs de descompresion
*/

typedef struct
{
unsigned int width; 
unsigned int height; 

unsigned char cant1;
unsigned char cant2;
unsigned char cant3;

unsigned char sepl1;
unsigned char sepl2;
unsigned char sepl3;

}DESC_FILEHEADER;

#endif