

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include "src/linkedlist.h"
#include "tabla_hash.h"
#include "bmpfile.h"
#include "hoffman.h"
#include "list.h"
#include "nodo_tree.h"
#include "skip_tree.h"
#include "bit.h"


/*
JAVIERA ASTUDILLO
BELÉN SALDÍAS


Se comprobó y no tenemos memory leak en ninguno de los dos modos, 

ya sea 0:comprimir o 1:descomprimir.

La compresión de hace perfectamente y completa.

La descompresión solo nos faltó interpretar los bits, pero la idea
se explica más abajo. En vez de ingresar al skipTree mediante un color
solicitando que retornase una codificación, luego de leer el header
de lo comprimido -cosa que si hacemos perfecto, se puede comprobar con
los prints! - se iba a entrar con una codificación al skipTree esperando
que retornase un color.

Este skipTree ya está "entrenado" con la lista de pares color-codificación
de header. Es decir, eso si lo hicimos.

Todo lo demás excepto la perfecta impresión de la nueva imagen
están implementados y perfectamente testeados.
De hecho, se genera una imagen con todo el header correcto al descomprimir, 
pero en negro ya que solo no alcanzamos a escribir los bits.
Todo lo demás está ok.

Los endianes de los test que subieron no coincidian con los del header 
del pdf, nos guiamos por lo del pdf ya que era lo oficial ,por eso se verá
que varias veces se dan vuelta los endianes.


Se compila con: tuNumeroDeAlumno$ gcc tarea2.c -std=c99 -o tarea2 -lm
*/


Tabla_encadenamiento* tabla0;


int main(int argc, char const *argv[])
{


	clock_t start = clock();

	if(!strcmp(argv[1],"0")){
		tabla0 = tabla_init(40, 0.9);

		FILEHEADER fileHeader;
		FILE *inbin;
		FILE *outbin;
		char image[2048];
		FILEHEADER data;
		INFOHEADER info_header;
		Color color;
		  
		snprintf(image,strlen(argv[2])+1,"%s",argv[2]);
		inbin = fopen(image, "rb");
		fread(&data,sizeof(unsigned char),14, inbin);
		fread(&info_header,sizeof(unsigned char),40, inbin);
		
		/*
		Hacer conteo de frecuencia absoluta. Se contó usando
		tabla de hash para que fuese más eficiente la cuenta.
		*/
		Element *e;
		char * imp;
		
	    int i = 0;
	    while (fread(&color,sizeof(unsigned char),3, inbin)==3)
	    {	
	   		imp = concat_color(color);
	   		e = init_Element(imp);
	   		table_add(tabla0, e);
	    }
		        
		fclose(inbin);

		//array_print(tabla0->array);

		/*
		Crear heap para posterior árbol
		*/

		Heap *heap = heap_init(tabla0->array->size+1, 2);

		Nodo *NodoCurrent;
		LinkedList *list;
		for (i = 0; i < tabla0->array->capacity; i++)
	    {
	        list = tabla0->array->content[i];
		    if (list->size > 0)
		    {
		        NodoCurrent = list->first;
		        while (NodoCurrent != NULL)
		        {
		        	basicInsertion(heap, NodoCurrent->element);
		            NodoCurrent = NodoCurrent->next;
		        }
		    }
	    }


	    buildHeap(heap);
	    print_heap(heap);


	    /*
		Construir árbol de Houffman
	    */
		Element *uno ;
		Element *dos ;
		Element *resultante;

		LinkedList *valiosos = list_init();
		LinkedList *temp = list_init();

		int u = 0;
		if(heap->size == 1){
			uno = xMin(heap);
			list_add(temp, uno);
			resultante = uno;

		}else{
			u = 1;
			while(heap->size >1){
			uno = xMin(heap);
			dos = xMin(heap);

			resultante = init_Element2(uno, dos);
			inserObject(heap, resultante);
			list_add(temp, resultante);

			list_add(temp , uno);
			list_add(temp, dos);
			
		}
		}

		/*
		Asignar codificaciones
		*/
		char * separator = malloc(sizeof(char)*(temp->size+2));
		asignar_codificacion(resultante, valiosos, separator, u);
		list_print(valiosos);
		printf("Separador:%s\n", separator);

		/*
		Creacion SkipTree
		*/
		

	    Stree* st = new_stree();
	    
	    int cantidad_cod =0; 
	    
	    if (valiosos->size > 0)
	    {
	        NodoCurrent = valiosos->first;
	        while(NodoCurrent!=NULL)
	        {
	        	cantidad_cod = cantidad_cod + 1;
	            stree_add_value(st, NodoCurrent->element->value, NodoCurrent->element->codification);
	            NodoCurrent = NodoCurrent->next;
	        }
	    }

	    /*
	    Comprimir
	    */

		snprintf(image,strlen(argv[2])+1,"%s",argv[2]);
		inbin = fopen(image, "rb");
		fread(&data,sizeof(unsigned char),14, inbin);
		fread(&info_header,sizeof(unsigned char),40, inbin);

		//Se genera el header del comprimido segun lo especificado en la tarea
		unsigned char byte1;
		unsigned char byte2;
		unsigned char byte3;
		unsigned char byte4;

		char image2[2048];
		snprintf(image2,strlen(argv[3])+1,"%s",argv[3]);
		outbin = fopen(image2, "wb");



		printf("WIDTH: %d\n",(info_header).width);
		printf("HEIGHT: %d\n", (info_header).height);
		printf("CODIFICACIONES: %d\n", cantidad_cod);
		//WIDTH
		byte1 = ((unsigned char*)(&info_header.width))[0];
		byte2 = ((unsigned char*)(&info_header.width))[1];
		byte3 = ((unsigned char*)(&info_header.width))[2];
		byte4 = ((unsigned char*)(&info_header.width))[3];
		fwrite(&byte4,sizeof(unsigned char),1, outbin);
		fwrite(&byte3,sizeof(unsigned char),1, outbin);
		fwrite(&byte2,sizeof(unsigned char),1, outbin);
		fwrite(&byte1,sizeof(unsigned char),1, outbin);

		//HEIGTH
		byte1 = ((unsigned char*)(&info_header.height))[0];
		byte2 = ((unsigned char*)(&info_header.height))[1];
		byte3 = ((unsigned char*)(&info_header.height))[2];
		byte4 = ((unsigned char*)(&info_header.height))[3];
		fwrite(&byte4,sizeof(unsigned char),1, outbin);
		fwrite(&byte3,sizeof(unsigned char),1, outbin);
		fwrite(&byte2,sizeof(unsigned char),1, outbin);
		fwrite(&byte1,sizeof(unsigned char),1, outbin);

		//CANTIDAD CODIFICACIONES
		byte1 = ((unsigned char*)(&cantidad_cod))[0];
		byte2 = ((unsigned char*)(&cantidad_cod))[1];
		byte3 = ((unsigned char*)(&cantidad_cod))[2];
		fwrite(&byte3,sizeof(unsigned char), 1, outbin);
		fwrite(&byte2,sizeof(unsigned char), 1, outbin);
		fwrite(&byte1,sizeof(unsigned char), 1, outbin);

		//SEPARADOR
		write_length_sep(separator, outbin);
		write_cod(separator,outbin);

		//Se escriben las tuplas <color, codificacion>, intercaladas por el separador, segun lo especificado en la entrega
		
		NodoJ* nodo;
	    if (valiosos->size > 0)
	    {
	    	//valiosos: lista con todas las tuplas determinadas por Hoffman
	        NodoCurrent = valiosos->first;
	        while(NodoCurrent!=NULL)
	        {
	        	nodo = search_key((*st).root, NodoCurrent->element->value);
	        	//color
	        	write_color((*nodo).valor, outbin);
	        	//codificacion
	        	write_cod((*nodo).valor_retorno, outbin);
	        	//separacion
	        	write_cod(separator, outbin);
	        	NodoCurrent = NodoCurrent->next;
	        }
	    }
	    
		printf("Iteraciones: %d\n", info_header.height*info_header.width);
		//Se lee el archivo original (luego de haberse saltado el header) y por cada color se escribe su codificaicon correspondiente
		//en el archivo comprimido.
	    while (fread(&color,sizeof(unsigned char),3, inbin)==3)
	    {	
	    	//Determinar color en archivo original
	   		imp = concat_color(color);
	   		//Buscar llave en el skip tree
	   		nodo = search_key((*st).root, imp);
	   		//Escribir codificacion en el archivo de salida
	   		write_cod((*nodo).valor_retorno, outbin);
	   		//Escribir separador
	   		write_cod(separator, outbin);
	   		free(imp);
	    }
	    flush_bits(outbin);
		        
		fclose(inbin);
		fclose(outbin);


		/*
		Free memory
		*/
		tree_destroy(st);
	    free(separator);
	    list_destroy(temp);
	    list_destroy2(valiosos);
		table_destroy(tabla0);
	    free(heap->content);
	    free(heap);

	}else if(!strcmp(argv[1],"1")){


		/*
		En esta primera parte se lee el archivo y
		se dan vuelta los endianes para cumplir con 
		el requisito de header.pdf subido por  los ayudantes.
		*/

		FILE *inbin;
		FILE *outbmp;
		char image[2048];
		  
		snprintf(image,strlen(argv[3])+1,"%s",argv[3]);
		inbin = fopen(image, "rb");

		DESC_FILEHEADER data;
		fread(&data,sizeof(unsigned char),14, inbin);
		//printf("a%i\n", data.width);
		//printf("b%i\n", data.height);


		int x = data.width;
	    x = ((x>>24)&0xff) | // move byte 3 to byte 0
                    ((x<<8)&0xff0000) | // move byte 1 to byte 2
                    ((x>>8)&0xff00) | // move byte 2 to byte 1
                    ((x<<24)&0xff000000); // byte 0 to byte 3
	    printf("ancho = %i\n",x);

	    int y = data.height;
	    y = ((y>>24)&0xff) | // move byte 3 to byte 0
                    ((y<<8)&0xff0000) | // move byte 1 to byte 2
                    ((y>>8)&0xff00) | // move byte 2 to byte 1
                    ((y<<24)&0xff000000); // byte 0 to byte 3

	    printf("alto = %i\n",y);

		int cant_codificaciones = 0;
		cant_codificaciones = (data.cant1 << 16) | (data.cant2 << 8) | data.cant3;
		printf("cantidad codificaciones :%i\n", cant_codificaciones);

		float sep_len = 0;
		sep_len = (data.sepl1 << 16) | (data.sepl2 << 8) | data.sepl3;
		int slen = (data.sepl1 << 16) | (data.sepl2 << 8) | data.sepl3;
		printf("largo separador :%i\n", slen);
		float cantidad_bytes = sep_len/8;
		//printf("%f\n", cantidad_bytes);
	
		unsigned char pre_separador[0];
		int largo = (int)cantidad_bytes;
		if (cantidad_bytes-(int)cantidad_bytes>0){
			largo = (int)cantidad_bytes + 1;
		}

		fread(pre_separador,sizeof(unsigned char),largo+1, inbin);


		/*
		En este arreglo se tienen todos los bits para posterior
		manipulación, detección del separador, etc.
		*/
		unsigned char *todos = malloc(sizeof(char*)*(x*y*8*3));
		int conta_todos = 0;

		/*como le dpy un byte más debo recordar cuál era
		así que lo guardo en la variable extra.
		*/
		unsigned char extra = pre_separador[largo];

		pre_separador[largo] = '\0';


		/*el separador nunca debería tener tamaño mayor a 20
		de hecho deberia ser siempre de largo alrededor de 3.
		*/

		unsigned char arr[5];	

		for (int k = 0; k < largo; ++k)
		{
			unsigned char n = pre_separador[k];
			printf("%i\n", n);

		    for(int i=0; i<8; i++) {
			    if (n & 1){
			    	arr[(k+1)*7-i] = '1';
			    }
			    else{
			    	arr[(k+1)*7-i] = '0';
		    	}

			    n >>= 1;
			}
		}


		/*
		Estos son los bits restandes de los bytes usados por
		el separador
		*/

		printf("No usados: ");

		for (int i = slen; i < largo*8; ++i)
		{
			unsigned char momento[2];
			momento[0] = arr[i];
			todos[conta_todos] = arr[i];
			conta_todos++;
			momento[1] = '\0';
			printf("%s", momento);
		}


		arr[slen] = '\0';
		printf("\nseparador :%s\n", arr);



		/*
		De aquí en adelante están los bites 
		que corresponden a la codificación e imagen:
		Los necesitamos al revés de como los estoy imprimiendo

		*/

		printf("otro = %i\n", extra);
	    for(int i=0; i<8; i++) {
	    	if(extra&1){
	    	todos[conta_todos] = '1';

	    	}else{
	    	todos[conta_todos] = '0';

	    	}
	    	conta_todos++;
		    extra >>= 1;
		}


		/*
		De aquí en adelante están los bites 
		que corresponden a la codificación e imagen:

		*/

		//printf("todos = \n");

		unsigned char arr2[1];
		int stop   = 1;
		while (stop==1)
		{
			stop = fread(arr2,sizeof(unsigned char),1, inbin);

			extra = arr2[0];
		    for(int hh=0; hh<8; hh++) {
		    	if (extra&1){
					todos[conta_todos] = '1';
		    	}else{
					todos[conta_todos] = '0';
		    	}
		    	conta_todos++;
			    extra >>= 1;
			}
		}


		/*
		En "todos" tenemos un arreglo de bits, este debería haber sido
		leído, preguntado al árbol y descomprimido.
		*/
		//printf("%s\n", todos);




		/*
		Este fragmento de código pretende reconocer el color de 3 bytes,
		su codificación y el separador.

		Funciona en algunos casos ya que no se temrinó de implementar
		completamente.

		La idea es que siemrpe el color será de 3 bytes, luego se lee hasta 
		encontrar un separador y los bits entre el color y el separador son 
		la codificación.

		*/
		int kk = 0;
		int conbit = 0;
		Stree* st = new_stree();
	    
	    int cantidad_cod =0; 
	   	char ** temporales = malloc(sizeof(char*)*cant_codificaciones*5);
	    int ug = 0;

		for (int gua = 0; gua < cant_codificaciones; ++gua)
		{
			char * current_color = malloc(sizeof(char)*24);
			conbit = kk;
			for (int i = 0; i < 24; ++i)
			{
				current_color[i] = todos[kk];
				kk++;
				conbit++;
			}
			//printf("\n%s:", current_color);
			char * current_cod =  malloc(sizeof(char)*24);
			for (int i = 0; i < 24; ++i)
			{
				if (i >= slen-1){
					int condi = 0;
					for (int j = 0; j < slen; ++j)
					{
						if (todos[kk-slen+j+1]!=arr[j]){		
							condi = 1;
							break;
						}
					}
					if (condi == 0){
							for (int uu = conbit; uu < kk-slen+2; ++uu)
							{
								current_cod[uu-conbit] = todos[uu];
							}
							current_cod[kk-slen-conbit+1] = '\0';
							//printf("%s\n", current_cod);


							/*

							OJO !! aquí hacemos la primera parte.
							*/
							stree_add_value(st, current_cod, current_color);

							break;
					}
				}
				kk++;
			}
			temporales[ug] = current_cod;
			ug++;
			temporales[ug] = current_color;
			ug++;
			//free (current_color);
			//free (current_cod);
		}

		/*
		Se supone que luego de que leímos todos los colores con sus 
		respctivas codificaciones vamos viendo si encontramos o no una codificacion.
		*/
		//sabemos que el contador va en kk.
		/*
		Por cada par codificacion, color hicimos: 
		stree_add_value(skip, codif, color);

		Luego deberíamos ir encontrando el separador retornara un color:
		search_key((*tree), codificacion);
		

		*/
		/*

		Primero escribamos el HEADER
		*/
		


	//HEADER
		//Escribir header del archivo descomprimido
		//outbmp = fopen("outbmp.bin", "wb");
		char image2[2048];
		snprintf(image2,strlen(argv[2])+1,"%s",argv[2]);
		outbmp = fopen(image2, "wb");
		//B
		unsigned char letra;
		letra = 0x42;
		fwrite(&letra,sizeof(unsigned char), 1, outbmp);
		//M
		letra = 0x4D;
		fwrite(&letra,sizeof(unsigned char), 1, outbmp);
		int numero;
		//Tamano en bytes del archivo BMP
		numero = 44+x*y*3;
		//printf("numero: %d\n", numero);
		fwrite(&numero,sizeof(int), 1, outbmp);
		numero = 0;
		fwrite(&numero,sizeof(int), 1, outbmp);
		//Offset del inicio de los datos
		numero = 54;
		fwrite(&numero,sizeof(int), 1, outbmp);
		//Tamano del header
		numero = 40;
		fwrite(&numero,sizeof(int), 1, outbmp);
		//Width
		numero = x;
		fwrite(&numero,sizeof(int), 1, outbmp);
		//Height
		numero = y;
		fwrite(&numero,sizeof(int), 1, outbmp);
		//Planos
		short n_short = 1;
		fwrite(&n_short,sizeof(short), 1, outbmp);
		//Bits por pixel
		n_short = 24;
		fwrite(&n_short,sizeof(short), 1, outbmp);
		//Metodo de compresion usado
		numero = 0;
		fwrite(&numero,sizeof(int), 1, outbmp);
		//Padding
		int padding = (x*3)%4;
		if(padding!=0){
			padding = 4 - padding;
		}
		//Tamano d ela imagen en bytes incluyendo padding
		numero = x*y*3 +padding*y;
		fwrite(&numero,sizeof(int), 1, outbmp);
		//Resolucion horizontal y vertical
		numero = 0xec4;
		fwrite(&numero,sizeof(int), 1, outbmp);
		fwrite(&numero,sizeof(int), 1, outbmp);
		//Numero de colores en la paleta y colores importantes
		numero = 0;
		fwrite(&numero,sizeof(int), 1, outbmp);
		fwrite(&numero,sizeof(int), 1, outbmp);



			/*



			Las siguientes líneas fueron las unicas que nos faltó implementar bien.


			*/

		int level = 1;
		NodoJ* nodo;
		char *codificacion_; 
		codificacion_ = malloc(sizeof(char)*level);
		sprintf(codificacion_, "%i", todos[kk]);


		while(kk>conta_todos){
	   		//Buscar llave en el skip tree
	   		nodo = search_key((*st).root, codificacion_);
			if (nodo != NULL){
				level=0;
		   		//Escribir el color el archivo de salida
		   		write_cod((*nodo).valor_retorno, outbmp);
				free(codificacion_);
	   		}
			level++;
			kk++;

			codificacion_ = malloc(sizeof(char)*level);
			sprintf(codificacion_, "%s%i", codificacion_, todos[kk]);

		}
		free(codificacion_);




		for (int i = 0; i < cant_codificaciones*5; ++i)
		{
			free(temporales[i]);
		}
		tree_destroy(st);
		free(temporales);



		fclose(outbmp);
		fclose(inbin);
		free(todos);

	}

	printf("\nTiempo transcurrido: %f\n", ((double)clock()-start)/CLOCKS_PER_SEC);
	return 0;
}