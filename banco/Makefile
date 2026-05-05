CC = gcc-4.1
CFLAGS = -Wall -pedantic-errors -std=gnu89 -O3 -I./ 
DEPCH= funciones.c funciones.h clave.c clave.h def.h global.c global.h cola.c cola.h
DEPC= funciones.c clave.c global.c cola.c
EJECUTABLES= banco cajero

all: clean $(EJECUTABLES)

banco: banco.c $(DEPCH)
	$(CC) $(CFLAGS) -o banco banco.c $(DEPC)

cajero: cajero.c $(DEPCH)
	$(CC) $(CFLAGS) -o cajero cajero.c $(DEPC)



clean:
	rm -f *.o $(EJECUTABLES) 

