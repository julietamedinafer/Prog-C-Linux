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


void *ThreadJugador(void *parametro){
    
    int num_animal, cant_pasos, cont_ronda, ganador;
    char nombre_animal[TAMANO_BUFFER];
    char msg_a_enviar[TAMANO_BUFFER];
    int fin= FALSE;
    mensaje msg;
    DatosAnimal *datos;

    /* variables locales del thread con datos del proceso principal */
    datos= (DatosAnimal*)parametro; /* puntero a la estructura de los datos */
    num_animal= datos->num_animal;
    cant_pasos= datos->cant_pasos;
    cont_ronda= datos->cont_ronda;
    strcpy(nombre_animal, datos->nombre_animal);
    ganador= datos->ganador;

    /* limpieza */
    memset(msg_a_enviar, 0x00, sizeof(msg_a_enviar));
    
    while(fin== FALSE){
        recibir_mensaje(id_cola, MSG_ANIMAL+num_animal, &msg);

        switch(msg.evento) {
            case EVT_INICIO:
                printf("Soy el %s\n", nombre_animal);  
                memset(msg_a_enviar, 0x00, sizeof(msg_a_enviar));
                sprintf(msg_a_enviar, "%d", num_animal);
                enviar_mensaje(id_cola, MSG_PISTA, MSG_ANIMAL+num_animal, EVT_INICIO, msg_a_enviar);
                usleep(ESPERAUS*1000);
            break;
            case EVT_CORRO:
                pthread_mutex_lock(&mutex);
                
                cont_ronda+= 1; 
                
                if(num_animal==0){ /* CONEJO */
                    cant_pasos+= obtener_aleatorio(4, 9);
                }
                if(num_animal==1){ /* GATO */
                    cant_pasos+= obtener_aleatorio(1, 10);
                }
                if(num_animal==2){ /* PERRO */
                    cant_pasos+= obtener_aleatorio(3, 8);
                }

                if(cant_pasos>= 117){
                    ganador= TRUE;
                } else {
                    ganador= FALSE;
                }

                memset(msg_a_enviar, 0x00, sizeof(msg_a_enviar));
                sprintf(msg_a_enviar, "%s %d %d %d", nombre_animal, cant_pasos, cont_ronda, ganador);

                printf("\nSoy el %s, avance %d pasos en la ronda %d", nombre_animal, cant_pasos, cont_ronda);
                    
                enviar_mensaje(id_cola, MSG_PISTA, MSG_ANIMAL+num_animal, EVT_CORRO, msg_a_enviar);
                pthread_mutex_unlock(&mutex);
                
                usleep(ESPERAUS*1000);
            break;
            case EVT_FIN:
                if(cant_pasos>= 117){
                    printf("\n\nSoy el %s: He ganado la carrera!\n", nombre_animal);
                }else{
                    printf("\nSoy el %s: He perdido la carrera!\n", nombre_animal);
                }
                fin = TRUE;
            break;
        }
    }

    pthread_exit((void *)"Listo");

}
