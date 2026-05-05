CC = gcc-4.1
CFLAGS = -Wall -pedantic-errors -std=gnu89 -O3 -I./ -pthread
DEPCH= archivos.c archivos.h semaforo.c semaforo.h funciones.c funciones.h clave.c clave.h def.h global.c global.h cola.c cola.h memoria.c memoria.h
DEPC= archivos.c semaforo.c funciones.c clave.c global.c cola.c memoria.c
EJECUTABLES= laberinto jugador 

all: clean $(EJECUTABLES)

laberinto: laberinto.c $(DEPCH)
	$(CC) $(CFLAGS) -o laberinto laberinto.c $(DEPC)

jugador: jugador.c $(DEPCH) thread.c thread.h
	$(CC) $(CFLAGS) -o jugador jugador.c $(DEPC) thread.c



clean:
	rm -f *.o $(EJECUTABLES) 

