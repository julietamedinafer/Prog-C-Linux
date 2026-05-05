CC = gcc-4.1
CFLAGS = -Wall -pedantic-errors -std=gnu89 -O3 -I./ 
DEPCH= funciones.c funciones.h semaforo.c  semaforo.h clave.c clave.h def.h global.c global.h memoria.c memoria.h
DEPC= funciones.c semaforo.c clave.c global.c memoria.c
EJECUTABLES= tienda vendedor

all: clean $(EJECUTABLES)

tienda: tienda.c $(DEPCH)
	$(CC) $(CFLAGS) -o tienda tienda.c $(DEPC)

vendedor: vendedor.c $(DEPCH)
	$(CC) $(CFLAGS) -o vendedor vendedor.c $(DEPC)



clean:
	rm -f *.o $(EJECUTABLES) 

