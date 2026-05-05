#ifndef SEMAFOROS_H
#define SEMAFOROS_H

int creo_semaforo();
void inicio_semaforo(int id_semaforo, int valor);
void levanta_semaforo(int id_semaforo);
void espera_semaforo(int id_semaforo);
#endif
