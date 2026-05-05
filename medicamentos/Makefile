CC = gcc-4.1
CFLAGS = -Wall -pedantic-errors -std=gnu89 -O3 -I./ 
DEPCH= archivos.c archivos.h funciones.c funciones.h semaforo.c  semaforo.h clave.c clave.h def.h global.c global.h
DEPC= archivos.c funciones.c semaforo.c clave.c global.c
EJECUTABLES= control medicamento

all: clean $(EJECUTABLES)

control: control.c $(DEPCH)
	$(CC) $(CFLAGS) -o control control.c $(DEPC)

medicamento: medicamento.c $(DEPCH)
	$(CC) $(CFLAGS) -o medicamento medicamento.c $(DEPC)



clean:
	rm -f *.o $(EJECUTABLES)

