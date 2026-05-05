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
HiloJUGADOR (uno por JUGADOR): (MSG_JUGADOR+nro_JUGADOR)
Para evitar que todas disparen juntas se deberá usar un MUTEX.

- Espera recibir el mensaje EVT_INICIO.
2- Escribir por pantalla, “Soy el jugador N y voy a dispararme“
3- Envía al REVOLVER el EVT_DISPARO, NUMERO_ELEGIDO; este número lo saca del
“vector_tambor”, de uno por vez (es decir, donde no hay un 0), después pone esa posición en 1.
vector_tambor=>[1][0][0][0][0][0].
4- Escribir por pantalla, “Soy el jugador N y la posición del tambor es M“
5- Espera recibir mensajes del REVOLVER.
6- SI recibe un mensaje EVT_SALVADO.
Si recibe un mensaje EVT_FIN, devuelve el número del JUGADOR “eliminado” al main.
7- Devuelve si se “salvó” o fue “eliminado” y termina*/

void *ThreadJugador(void *parametro){
    int num_jugador;
    int id_cola_mensajes;
    int bala_elegida;
    char msg_a_enviar[TAMANO_BUFFER];
    int fin= FALSE;
    int *vector;
    mensaje msg;
    DatosJugador *datos;

    /* variables locales del thread con datos del proceso principal */
    datos= (DatosJugador*)parametro;
    vector= datos->vector_tambor;
    num_jugador= datos->num_jugador;
    id_cola_mensajes= datos->id_cola_mensaje;
    
    while(fin== FALSE) {
        recibir_mensaje(id_cola_mensajes, MSG_JUGADOR+num_jugador, &msg);
        switch(msg.evento) {
            case EVT_INICIO:
                pthread_mutex_lock(&mutex);
                bala_elegida = obtener_aleatorio(0,5);
                while(1) {
                    if (vector[bala_elegida] != 0) {
                        bala_elegida = obtener_aleatorio(0,5);
                    } else {
                        vector[bala_elegida] = 1;
                        break;
                    }
                }
                printf("Soy el jugador %d y voy a dispararme con la bala %d\n", num_jugador, bala_elegida);
                pthread_mutex_unlock(&mutex);
                
                sprintf(msg_a_enviar, "%d", bala_elegida);
                enviar_mensaje(id_cola_mensajes, MSG_REVOLVER, MSG_JUGADOR+num_jugador, EVT_DISPARO, msg_a_enviar);
            break;
            case EVT_SALVADO:
                printf("Jugador %d: me salve\n", num_jugador);
                break;
            case EVT_FIN:
                if (atoi(msg.contenidoMsg) == num_jugador) {
                    printf("Jugador %d: mori\n", num_jugador);
                    datos->fue_eliminado = TRUE;
                }
                fin= TRUE;
                break;
        }
    }

    pthread_exit((void *)"Listo");

}
