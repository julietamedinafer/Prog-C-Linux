CC = gcc-4.1
CFLAGS = -Wall -pedantic-errors -std=gnu89 -O3 -I./ -pthread
DEPCH= funciones.c funciones.h clave.c clave.h def.h global.c global.h cola.c cola.h
DEPC= funciones.c clave.c global.c cola.c
EJECUTABLES= revolver jugadores

all: clean $(EJECUTABLES)

revolver: revolver.c $(DEPCH)
	$(CC) $(CFLAGS) -o revolver revolver.c $(DEPC)

jugadores: jugadores.c $(DEPCH) thread.c thread.h
	$(CC) $(CFLAGS) -o jugadores jugadores.c $(DEPC) thread.c



clean:
	rm -f *.o $(EJECUTABLES) 

