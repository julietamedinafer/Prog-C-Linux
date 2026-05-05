#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include "def.h"
#include "clave.h"
#include "global.h"
#include "cola.h" 
#include "funciones.h"
#include "thread.h"
#include "archivos.h"
#include "semaforo.h"


void *ThreadJugador(void *parametro){
    
    int id_caja, total, total_acumulado= 0;
    char msg_a_enviar[TAMANO_BUFFER];
    char nombre_archivo[TAMANO_BUFFER];
    int fin= FALSE;
    mensaje msg; 
    DatosSuper *datos;

    /* variables locales del thread con datos del proceso principal */
    datos= (DatosSuper*)parametro; /* puntero a la estructura de los datos */
    id_caja= datos->id_caja;
    total= datos->total;

    /* limpieza */
    memset(msg_a_enviar, 0x00, sizeof(msg_a_enviar));
    
    while(fin== FALSE){
        recibir_mensaje(id_cola, MSG_CAJAS+id_caja, &msg);

        switch(msg.evento) {
            case EVT_INICIO:  
                printf("Caja %d inicializada\n", id_caja+1);
            break;
            case EVT_COBRAR:
                pthread_mutex_lock(&mutex);
                sscanf(msg.contenidoMsg, "%d", &total); 
                total_acumulado+= total;
                
                printf("Caja %d - Total a cobrar: $ %d\n", id_caja+1, total);
                    
                espera_semaforo(id_semaforo);
                sprintf(nombre_archivo, "caja_%d.txt", id_caja+1);
                abrirArchivo(nombre_archivo, "w");
                sprintf(linea, "Total caja %d: $ %d", id_caja+1, total_acumulado);
                escribirArchivo(linea);
                cerrarArchivo();
                abrirArchivo(nombre_archivo, "r");
                leerArchivo(nombre_archivo);
                cerrarArchivo();
                levanta_semaforo(id_semaforo);

                pthread_mutex_unlock(&mutex);
            break;
            case EVT_FIN: 
                fin= TRUE;
            break;
        }
    }

    pthread_exit((void *)"Listo");

}
