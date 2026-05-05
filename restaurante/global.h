#ifndef GLOBAL_H
#define GLOBAL_H

#include <pthread.h>

extern int id_semaforo;
extern int id_memoria;
extern char linea[256];
extern int id_cola;
extern pthread_t *id_hilo;
extern pthread_attr_t attr;
extern pthread_mutex_t mutex;

#endif
