#a futuro incluir -pthread en CFLAGS
#incluir archivos de lectura creados en el clean
CC=gcc-4.1 
CFLAGS= -Wall -O3 -std=gnu89 -pedantic-errors -I./ 

all: clean Programa1 Programa2 

Programa1:
	$(CC) $(CFLAGS) -o Programa1 main1.c funciones.c global.c archivos.c clave.c semaforos.c

Programa2: 
	$(CC) $(CFLAGS) -o Programa2 main2.c funciones.c global.c archivos.c clave.c semaforos.c

clean: 
	rm -f *.o Programa1 Programa2 