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

/*CONSIGNA:
Hilo JUGADOR (uno por JUGADOR): (MSG_JUGADOR + nro_JUGADOR)
1) Espera recibir el EVT_INICIO.
2) Escribir por pantalla, “Soy el jugador N y voy a tirar“.
3) Envía al BOLERA el EVT_TIRO.
4) Espera recibir el EVT_RESULTADO o EVT_FIN.
5) Muestra por pantalla los bolos tirados “Soy el jugador N y tire X bolos”.
6) Espera 300 ms y vuelve a tirar.
7) Se recibe el EVT_FIN; termina el hilo devolviendo los puntos obtenidos.
8) Muestra por pantalla los puntos obtenidos.*/


void *ThreadJugador(void *parametro){
    
    int id_participante, cant_bolos, meta, puntos;
    char msg_a_enviar[TAMANO_BUFFER];
    int fin= FALSE;
    mensaje msg; 
    DatosParticipante *datos;

    /* variables locales del thread con datos del proceso principal */
    datos= (DatosParticipante*)parametro; /* puntero a la estructura de los datos */
    id_participante= datos->id_participante;
    puntos= datos->puntos;

    /* limpieza */
    memset(msg_a_enviar, 0x00, sizeof(msg_a_enviar));
    
    while(fin== FALSE){
        recibir_mensaje(id_cola, MSG_PARTICIPANTE+id_participante, &msg);

        switch(msg.evento){
            case EVT_INICIO:
                sscanf(msg.contenidoMsg, "%d", &meta);
                printf("\nSoy el participante %d listo para tirar %d bolos\n", id_participante+1, meta);
                enviar_mensaje(id_cola, MSG_LUGAR, MSG_PARTICIPANTE+id_participante, EVT_TIRO, "");
                
                usleep(ESPERAUS*1000);
            break;
            
            case EVT_RESULTADO:
                sscanf(msg.contenidoMsg, "%d %d", &cant_bolos, &puntos);
                printf("Participante %d tire %d bolos. Puntos acumulados: %d\n", id_participante+1, cant_bolos, puntos);
                    
                /* acceso a recursos compartidos */
                pthread_mutex_lock(&mutex);
                datos->puntos= puntos;
                pthread_mutex_unlock(&mutex);
                    
                usleep(ESPERAUS*1000);
                enviar_mensaje(id_cola, MSG_LUGAR, MSG_PARTICIPANTE+id_participante, EVT_TIRO, "");
            break;
            
            case EVT_FIN:
                sscanf(msg.contenidoMsg, "%d %d", &cant_bolos, &puntos);
                printf("Participante %d tire todos los bolos, puntos obtenidos: %d\n", id_participante+1, puntos);
                fin= TRUE;
            break;
        }
    }

    pthread_exit((void *)"Listo");

}
