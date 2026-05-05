CC = gcc-4.1
CFLAGS = -Wall -pedantic-errors -std=gnu89 -O3 -I./ 
DEPCH= funciones.c funciones.h semaforo.c  semaforo.h clave.c clave.h def.h global.c global.h memoria.c memoria.h
DEPC= funciones.c semaforo.c clave.c global.c memoria.c
EJECUTABLES= pienso jugador

all: clean $(EJECUTABLES)

pienso: pienso.c $(DEPCH)
	$(CC) $(CFLAGS) -o pienso pienso.c $(DEPC)

jugador: jugador.c $(DEPCH)
	$(CC) $(CFLAGS) -o jugador jugador.c $(DEPC)



clean:
	rm -f *.o $(EJECUTABLES) 

