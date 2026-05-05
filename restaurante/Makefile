CC = gcc-4.1
CFLAGS = -Wall -pedantic-errors -std=gnu89 -O3 -I./ -pthread
DEPCH= archivos.c archivos.h semaforo.c semaforo.h funciones.c funciones.h clave.c clave.h def.h global.c global.h cola.c cola.h memoria.c memoria.h
DEPC= archivos.c semaforo.c funciones.c clave.c global.c cola.c memoria.c
EJECUTABLES= derivador mesero 

all: clean $(EJECUTABLES)

derivador: derivador.c $(DEPCH)
	$(CC) $(CFLAGS) -o derivador derivador.c $(DEPC)

mesero: mesero.c $(DEPCH) thread.c thread.h
	$(CC) $(CFLAGS) -o mesero mesero.c $(DEPC) thread.c



clean:
	rm -f *.o $(EJECUTABLES) 

