#ifndef MEMORIAH
#define MEMORIAH

void* creo_memoria(int size, int* r_id_memoria, int clave_base);
void libero_memoria(void* ptr_memoria, int id_memoria);

#endif
