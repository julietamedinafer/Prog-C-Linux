CC = gcc-4.1
CFLAGS = -Wall -pedantic-errors -std=gnu89 -O3 -I./ 
DEPCH= archivos.c archivos.h funciones.c funciones.h semaforo.c  semaforo.h clave.c clave.h def.h global.c global.h
DEPC= archivos.c funciones.c semaforo.c clave.c global.c
EJECUTABLES= control comida

all: clean $(EJECUTABLES)

control: control.c $(DEPCH)
	$(CC) $(CFLAGS) -o control control.c $(DEPC)

comida: comida.c $(DEPCH)
	$(CC) $(CFLAGS) -o comida comida.c $(DEPC)



clean:
	rm -f *.o $(EJECUTABLES) 

