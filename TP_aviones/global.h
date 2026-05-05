#ifndef GLOBAL_H
#define GLOBAL_H

typedef struct ReservaVuelo {
    int vuelo;               
    char destino[30];
    char nombre[50];
} Reserva;


extern Reserva r;
#endif
