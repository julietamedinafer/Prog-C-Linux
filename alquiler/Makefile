CC = gcc-4.1
CFLAGS = -Wall -pedantic-errors -std=gnu89 -O3 -I./ 
DEPCH= archivos.c archivos.h funciones.c funciones.h semaforo.c  semaforo.h clave.c clave.h def.h global.c global.h
DEPC= archivos.c funciones.c semaforo.c clave.c global.c
EJECUTABLES= sistema interesado

all: clean $(EJECUTABLES)

sistema: sistema.c $(DEPCH)
	$(CC) $(CFLAGS) -o sistema sistema.c $(DEPC)

interesado: interesado.c $(DEPCH)
	$(CC) $(CFLAGS) -o interesado interesado.c $(DEPC)



clean:
	rm -f *.o $(EJECUTABLES) 

