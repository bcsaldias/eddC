//#include <stdio.h>


/*
Método que representa el 'merge' que se realiza
en el MergeSort.
*/
void merge(int A[][3], int temp[][3], int e, int m, int w, int col){
	int p = e;
	int k = e;
	int q = m;
	while (p < m && q <= w){
		if (A[p][col] <= A[q][col]){
			temp[k][0] = A[p][0];
			temp[k][1] = A[p][1];
			temp[k][2] = A[p][2];
			k++;
			p++;
		}else{
			temp[k][0] = A[q][0];
			temp[k][1] = A[q][1];
			temp[k][2] = A[q][2];
			k++;
			q++;
		}
	}
	while (p < m){
			temp[k][0] = A[p][0];
			temp[k][1] = A[p][1];
			temp[k][2] = A[p][2];
			k++;
			p++;
	}
	while (q <= w){
			temp[k][0] = A[q][0];
			temp[k][1] = A[q][1];
			temp[k][2] = A[q][2];
			k++;
			q++;
	}
	for (k = e; k<=w; k++)
	{
		A[k][0] = temp[k][0];
		A[k][1] = temp[k][1];
		A[k][2] = temp[k][2];
	}
}

/*
Método para realizar mergeSort
*/
void mergeSort(int A[][3], int temp[][3], int e, int w, int col){
	if(e < w){
		int m = (w+e)/2;
		mergeSort(A, temp, e, m, col);
		mergeSort(A, temp, m+1, w, col);
		if (A[m][col] >= A[m+1][col])
			merge(A, temp, e, m+1, w, col);
	}
}

int solution(int A[], int N){
	/*
		El problema se consideró como intersección entre
		intervalos en el eje X. Es decir, se calculan las
		intersecciones solo en el eje X. El problema se
		reduce a intersección de intervalos más que a 
		intersección de discos en el espacio, ya que sabemos
		que todos tienen su coordenada Y en 0.
	*/

	/*
	En el array posiciones se guardará:
	posiciones[i][0] 	= 	i - radio[i] ó i + radio[i], es decir,
			 				guarda el inicios y final del disco i.
	posiciones[i][1]	= 	guarda un 0 si es en A[i][0] hay un inicio,
			  				guarda un 1 si hay un final.
	posiciones[i][2]	=	guarda el centro del disco, es decir, su id.
	*/

	int posiciones[N*2][3];
	int temp[N*2][3];

	/*
	Inicialización de 'posiciones'
	*/
	for (int i = 0; i < N; i+=1)
	{
		posiciones[i][0] = i-A[i];
		posiciones[N + i][0] = i+A[i];
		posiciones[i][1] = 0;
		posiciones[N + i][1] = 1;
		posiciones[i][2] = i;
		posiciones[N + i][2] = i;

		temp[i][0] = 0;
		temp[N + i][0] = 0;
		temp[i][1] = 0;
		temp[N + i][1] = 0;
		temp[i][2] = 0;
		temp[N + i][2] = 0;
	}

	/*
	El primer lugar se ordenan de acuerdo a si son 
	puntos de inicio o fin, quedando así todos los 
	inicios de discos primero en el arreglo que los
	finales.
	*/
	mergeSort(posiciones, temp, 0, 2*N-1, 1);

	/*
	Luego se ordenan correlativamente los valores 
	de los inicios y finales, es decir, los valores 
	de posiciones[i][0]. Debido a que anteriormente
	fueron ordenados por tipo inicio/fin, si hay dos
	valores iguales en posiciones[i][0] quedarán
	ordenados correlativamente los inicios antes de
	los finales
	*/
	mergeSort(posiciones, temp, 0, 2*N-1, 0);


	int intersecciones = 0;
	int acumulado = 0;
	int i = 0;
	int pivoteado = 0;
	int contador = 0;
	int inicios = 0;

	/*
	Para calcular el número de intersecciones, 
	se calcula cuántos puntos de inicios hay 
	entre el punto de inicio y el punto final 
	de cada disco, calculandolo en orden de
	aparición en el arreglo 'posiiones'. Estos 
	valores son sumados.
	*/
	int coo = N-1;
	int maximos_a_encontrar = N;
	while (inicios < coo && contador < 2*N){

		if (posiciones[contador][1] == 0){
			i = contador+1;
			acumulado = 0;
			inicios++;
			maximos_a_encontrar--;
			pivoteado = posiciones[contador][2];


			/*
			while desde el inicio de un círculo
			hasta llegar a su posición final.
			*/
			while (i < 2*N && (posiciones[i][2] != pivoteado &&
				maximos_a_encontrar > 0		)){
				if (posiciones[i][1] == 0){
					acumulado++;
				}
				i++;
			}

			intersecciones+= acumulado;
			if (intersecciones > 10000000){
				return -1;
			}
		}
		
		contador++;

	}

	return intersecciones;
}
/*
int main(int argc, char const *argv[])
{

	int A[] = {1, 5, 2, 1, 4, 0};
	printf("%i\n",solution( A, 6));

	int A1[] = {1};
	printf("%i\n",solution( A1, 1));


	int A2[] = {1, 1, 1};
	printf("%i\n",solution( A2, 3));

	int A3[] = {1, 10, 100, 1};
	printf("%i\n",solution( A3, 4));

	int A4[] = {1, 0, 1, 0, 1};
	printf("%i\n",solution( A4, 5));

	return 0;
}*/