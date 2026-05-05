#ifndef _SEM_H
#define _SEM_H

int creo_semaforo();
void inicia_semaforo(int id_semaforo, int valor);
void levanta_semaforo(int id_semaforo);
void espera_semaforo(int id_semaforo);

#endif
