CC = gcc-4.1
CFLAGS = -Wall -pedantic-errors -std=gnu89 -O3 -I./ -pthread
DEPCH= archivos.c archivos.h semaforo.c semaforo.h funciones.c funciones.h clave.c clave.h def.h global.c global.h cola.c cola.h
DEPC= archivos.c semaforo.c funciones.c clave.c global.c cola.c
EJECUTABLES= panel animales 

all: clean $(EJECUTABLES)

panel: panel.c $(DEPCH)
	$(CC) $(CFLAGS) -o panel panel.c $(DEPC)

animales: animales.c $(DEPCH) thread.c thread.h
	$(CC) $(CFLAGS) -o animales animales.c $(DEPC) thread.c



clean:
	rm -f *.o $(EJECUTABLES) 

