#include "global.h"
#include <pthread.h>

int id_semaforo;
int id_memoria;
char linea[256];
int id_cola;
pthread_t *id_hilo;
pthread_attr_t attr;
pthread_mutex_t mutex;
