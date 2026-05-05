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
    
    int id_jugador, posicion, cant_pasos, saldo, ganador;
    char msg_a_enviar[TAMANO_BUFFER];
    int fin= FALSE;
    mensaje msg; 
    DatosJugador *datos;
    int pierde_turno = FALSE;

    /* variables locales del thread con datos del proceso principal */
    datos= (DatosJugador*)parametro; /* puntero a la estructura de los datos */
    id_jugador= datos->id_jugador;
    posicion= datos->posicion;
    cant_pasos= datos->cant_pasos;
    saldo= datos->saldo;
    ganador= datos->ganador;

    /* limpieza */
    memset(msg_a_enviar, 0x00, sizeof(msg_a_enviar));
    
    while(fin== FALSE){
        recibir_mensaje(id_cola, MSG_JUGADOR+id_jugador, &msg);

        switch(msg.evento){
            case EVT_INICIO:
                printf("\n\nSoy el jugador %d\n", id_jugador+1);
                pthread_mutex_lock(&mutex);
                
                datos->posicion = posicion;
                datos->saldo = saldo;
                pthread_mutex_unlock(&mutex);
                
                enviar_mensaje(id_cola, MSG_TABLERO, MSG_JUGADOR+id_jugador, EVT_INICIO, "");
                
                usleep(ESPERAUS*1000);
            break;
            
            case EVT_CORRO:
                if(pierde_turno== FALSE){
                    cant_pasos= obtener_aleatorio(0, 3);
                    
                    printf("\nJugador %d: tiro los dados y obtengo %d pasos\n", id_jugador+1, cant_pasos);
                    
                    posicion+= cant_pasos;

                    if(posicion>= 60){
                        printf("\nJugador %d: ganeee!\n", id_jugador+1);
                        ganador= TRUE;
                        posicion= 60;
                        fin= TRUE; 
                    }else{
                        saldo= aplicar_posicion(posicion, saldo, id_jugador);

                        if(posicion== 14 || posicion== 53){
                            pierde_turno= TRUE;
                            printf("\nJugador %d: perdi el siguiente turno\n", id_jugador+1);
                        }
                    }
                    
                    if(saldo<= 0){
                        printf("\nJugador %d: me quede sin saldo, perdi el juego\n", id_jugador+1);
                        saldo= 0;
                        fin= TRUE;
                    }

                    printf("\nJugador %d: saldo actual $%d\n", id_jugador+1, saldo);
                }else{
                    printf("\nJugador %d: turno perdido\n", id_jugador+1);
                    pierde_turno= FALSE;
                }
                
                pthread_mutex_lock(&mutex);
                datos->posicion = posicion;
                datos->cant_pasos = cant_pasos;
                datos->saldo = saldo;
                datos->ganador = ganador;
                pthread_mutex_unlock(&mutex);
                
                sprintf(msg_a_enviar, "%d %d %d %d", posicion, saldo, ganador, fin);
                enviar_mensaje(id_cola, MSG_TABLERO, MSG_JUGADOR+id_jugador, EVT_CORRO, msg_a_enviar);
                
                usleep(ESPERAUS*1000);
            break;
            
            case EVT_FIN:
                printf("\nJugador %d: turno finalizado\n", id_jugador+1);
                enviar_mensaje(id_cola, MSG_TABLERO, MSG_JUGADOR+id_jugador, EVT_FIN, "");
                fin= TRUE;
            break;
        }
    }

    pthread_exit((void *)"Listo");

}
