#ifndef HASH_FUNCTIONS_EDD
#define HASH_FUNCTIONS_EDD

#include <math.h>

/*
	No modifiques ni una parte de este codigo, ni lo copies a tu tarea. Para usarlo deberas guardarlo en la misma
	carpeta e importarlo usando #include "hash.h" Luego de eso puedes usar estas funciones como si fueran tuyas.
	Esto es importante ya que a la hora de probar las tareas los parametros de las funciones cambiaran
*/
/*	Estas constantes (A,B,P) son importantes para la funcion de hash universal standard
	Si las redefines en tu codigo la funcion ya no servira de nada */
#define A 168151
#define B 157181
#define P 986437
/*
	Hasheo de texto, la funcion que deberas usar siempre
*/

int hash(int *numero)
{
	unsigned long hash = 0;

	/*for (int i = 0; i < 7; ++i)
	{
		printf("%d, ", numero[i]);
	}*/

	int binario[14];
	for (int i = 0; i < 14; ++i)
	{
		binario[i] = 0;
	}

	for (int i = 6; i >= 0; --i)
	{
		if (numero[i] == 1){
			binario[2*i + 1] = 1;
		}else if (numero[i] == 2){
			binario[2*i] = 1;
		}else if (numero[i] == 3){
			binario[2*i +1 ] = 1;
			binario[2*i] = 1;
		}
	}
	/*
	printf("binario: ");
	for (int i = 0; i < 14; ++i)
	{
		printf("%d", binario[i]);
	}*/

	for (int i = 13; i >= 0; --i)
	{
		hash += binario[i]*pow(2, 13-i);
	}

	/*printf(" numero:%d\n", hash);*/

	return hash;
}
/*
	Hasheo de numeros, tu funcion secundaria para probar el doble hashing.
	Utiliza el resultado de la anterior	como parametro.
*/
unsigned long universal(unsigned long k)
{
	return (A*k+B)%P;
}
#endif