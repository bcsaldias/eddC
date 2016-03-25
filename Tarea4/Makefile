# Compilar con estándares más modernos
CC=gcc -std=gnu99
# Optimizaciones nivel 3. Advertir todo.
CFLAGS=-O3 -Wall -Wunused-result
# El nombre de nuestro archivo que contiene el main, sin extension
MAIN=tarea4
# El archivo que tiene al main, con extension
SRC=$(MAIN).c
# El nombre de nuestro archivo de salida
OUT=tarea4

# Primero limpia el directorio, luego compila
all: clean compile

# Compilar
compile: $(MAIN).o
	$(CC) $(CFLAGS) -o $(OUT) $(MAIN).o -lm && rm -f $(MAIN).o

# Eliminar el resultado de la compilacion e imagenes
clean:
	rm -f $(OUT) && rm -f *.bmp
# (Borra el "&& rm -f *.bmp" si no quieres eliminar las imagenes)
