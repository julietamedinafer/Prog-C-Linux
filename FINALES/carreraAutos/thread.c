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
    
    int id_auto, posicion, cant_pasos, meta, cupo;
    int i, id_ganador;
    char msg_a_enviar[TAMANO_BUFFER];
    int fin= FALSE;
    mensaje msg; 
    DatosAuto *datos;

    /* variables locales del thread con datos del proceso principal */
    datos= (DatosAuto*)parametro; /* puntero a la estructura de los datos */
    id_auto= datos->id_auto;
    posicion= datos->posicion;
    cant_pasos= datos->cant_pasos;

    /* limpieza */
    memset(msg_a_enviar, 0x00, sizeof(msg_a_enviar));
    
    while(fin== FALSE){
        recibir_mensaje(id_cola, MSG_AUTO+id_auto, &msg);

        switch(msg.evento){
            case EVT_INICIO:
                sscanf(msg.contenidoMsg, "%d %d", &meta, &cupo);
                printf("\nSoy el auto %d listo para correr los %dm\n", id_auto+1, meta);
                pthread_mutex_lock(&mutex);
                
                datos->posicion= posicion;
                pthread_mutex_unlock(&mutex);
                
                enviar_mensaje(id_cola, MSG_CIRCUITO, MSG_AUTO+id_auto, EVT_NINGUNO, "");
                
                usleep(ESPERAUS*1000);
            break;
            
            case EVT_AVANZAR:
                if(fin== FALSE){
                    posicion= obtener_aleatorio(10, 100);
                    cant_pasos+= posicion;
                    printf("Auto %d avanzo %dm\nTotal recorrido %dm\n\n", id_auto+1, posicion, cant_pasos);
                    memset(msg_a_enviar, 0x00, sizeof(msg_a_enviar));
                    sprintf(msg_a_enviar, "%d %d", posicion, cant_pasos);

                    if(cant_pasos>= meta){
                        printf("Auto %d alcanzo la meta!\n", id_auto+1);
                        
                        /* avisar a todos los otros que perdieron */
                        for(i= 0; i< cupo; i++){
                            if(i!= id_auto){
                                memset(msg_a_enviar, 0x00, sizeof(msg_a_enviar));
                                sprintf(msg_a_enviar, "%d", id_auto+1);
                                enviar_mensaje(id_cola, MSG_AUTO+i, MSG_AUTO+id_auto, EVT_FIN, msg_a_enviar);
                            }
                        }
                        
                        memset(msg_a_enviar, 0x00, sizeof(msg_a_enviar));
                        sprintf(msg_a_enviar, "%d", id_auto+1);
                        enviar_mensaje(id_cola, MSG_CIRCUITO, MSG_AUTO+id_auto, EVT_GANADOR, msg_a_enviar);
                        
                        fin= TRUE;
                    }else{
                        enviar_mensaje(id_cola, MSG_CIRCUITO, MSG_AUTO+id_auto, EVT_AVANZAR, msg_a_enviar);
                    }
                    
                    /* acceso a recursos compartidos */
                    pthread_mutex_lock(&mutex);
                    datos->posicion= posicion;
                    datos->cant_pasos= cant_pasos;
                    pthread_mutex_unlock(&mutex);
                    
                    usleep(ESPERAUS*1000);
                }
            break;
            
            case EVT_INFO_CARRERA:
                /* Comunicacion bidireccional: recibir informacion del circuito */
                printf("Auto %d recibe del circuito: %s\n", id_auto+1, msg.contenidoMsg);
            break;
            
            case EVT_FIN:
                sscanf(msg.contenidoMsg, "%d", &id_ganador);
                printf("Auto %d recibe notificacion: Auto %d gano la carrera\n", id_auto+1, id_ganador);
                fin= TRUE;
            break;
        }
    }

    pthread_exit((void *)"Listo");

}
