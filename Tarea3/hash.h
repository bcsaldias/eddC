#ifndef HASH_FUNCTIONS_EDD
#define HASH_FUNCTIONS_EDD
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
unsigned long hash(unsigned char *str)
{
	unsigned long hash = 5381;
	int c;

	while (c = *str++)
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

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