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
    
    int id_jugador, posicion, cont_avance, vivo, camino_elegido, meta_final, avance, camino, id_ganador;
    int ataque_criatura, defensa_jugador, resultado_pared, salto;
    int cant_participantes;
    char msg_a_enviar[TAMANO_BUFFER];
    int fin= FALSE;
    mensaje msg; 
    DatosParticipante *datos;

    /* variables locales del thread con datos del proceso principal */
    datos= (DatosParticipante*)parametro; /* puntero a la estructura de los datos */
    id_jugador= datos->id_participante;
    posicion= datos->posicion;
    cont_avance= datos->cont_avance;
    vivo= datos->vivo;
    camino_elegido= datos->camino_elegido;
    meta_final= datos->meta_final;

    /* limpieza */
    memset(msg_a_enviar, 0x00, sizeof(msg_a_enviar));
    
    while(fin== FALSE){
        recibir_mensaje(id_cola, MSG_JUGADOR+id_jugador, &msg);

        switch(msg.evento){
            case EVT_INICIO:
                sscanf(msg.contenidoMsg, "%d %d", &meta_final, &cant_participantes); 
                printf("Muchacho %d: listo para recorrer %dm\n", id_jugador+1, meta_final);
                
                pthread_mutex_lock(&mutex);
                datos->vivo= vivo;
                datos->meta_final= meta_final;
                pthread_mutex_unlock(&mutex);
                
                usleep(ESPERAUS*500);
            break;
            
            case EVT_ATAQUE_CRIATURA:
                if(vivo== 1){
                    pthread_mutex_lock(&mutex);
                    
                    sscanf(msg.contenidoMsg, "%d", &ataque_criatura);
                    defensa_jugador= obtener_aleatorio(1, 2);
                    printf("Muchacho %d: Criatura ataca con %d, me defiendo con %d\n", id_jugador+1, ataque_criatura, defensa_jugador);
                    
                    if(ataque_criatura!= defensa_jugador){
                    printf("Muchacho %d: Me golpearon! Regreso a la isla para siempre!\n\n", id_jugador+1);
                        vivo= 0;
                    }else{
                        printf("Muchacho %d: Me defendi del ataque!\n", id_jugador+1);
                        
                        avance= obtener_aleatorio(1, 5);
                        cont_avance+= avance;
                        
                        printf("Muchacho %d: Avanzo %dm, posicion total: %dm\n\n", id_jugador+1, avance, cont_avance);
                        
                        /* bifurcacion */
                        if(cont_avance>= 20 && camino_elegido== 2){ /* por defecto elegi 2*/
                            camino= obtener_aleatorio(2, 3);
                            if(camino== 3){
                                meta_final= 180;
                                camino_elegido= 3;
                                printf("Muchacho %d: Tomo el camino largo (180m)\n", id_jugador+1);
                            }
                        }
                        
                        /* pared */
                        if(cont_avance>= 56){
                            resultado_pared= obtener_aleatorio(1, 7);
                            if(resultado_pared< 4){
                                printf("Muchacho %d: La pared se cerro y mori\n", id_jugador+1);
                                vivo= 0;
                            }else{
                                printf("Muchacho %d: Pase la pared\n", id_jugador+1);
                            }
                        }
                        
                        /* precipicio */ 
                        if(vivo== 1 && cont_avance>= 120){
                            salto= obtener_aleatorio(10, 20);
                            if(salto< 15){
                                printf("Muchacho %d: No pude saltar el precipicio y mori\n", id_jugador+1);
                                vivo= 0;
                            }else{
                                printf("Muchacho %d: Salte el precipicio\n", id_jugador+1);
                            }
                        }
                        
                        posicion= avance;
                    }
                    
                    /* actualizar datos compartidos */
                    datos->vivo= vivo;
                    datos->posicion= posicion;
                    datos->cont_avance= cont_avance;
                    datos->camino_elegido= camino_elegido;
                    datos->meta_final= meta_final;
                    
                    pthread_mutex_unlock(&mutex);
                    
                    memset(msg_a_enviar, 0x00, sizeof(msg_a_enviar));
                    sprintf(msg_a_enviar, "%d %d %d %d %d", vivo, posicion, cont_avance, camino_elegido, meta_final);
                    enviar_mensaje(id_cola, MSG_LABERINTO, MSG_JUGADOR+id_jugador, EVT_AVANZAR, msg_a_enviar);
                    
                    if(cont_avance>= meta_final && vivo== 1){
                        printf("Muchacho %d: ESCAPE DEL LABERINTO!!!\n", id_jugador+1);
                        fin= TRUE;
                    }
                    
                    usleep(ESPERAUS*500);
                }else{
                    memset(msg_a_enviar, 0x00, sizeof(msg_a_enviar));
                    sprintf(msg_a_enviar, "%d %d %d %d %d", vivo, posicion, cont_avance, camino_elegido, meta_final);
                    enviar_mensaje(id_cola, MSG_LABERINTO, MSG_JUGADOR+id_jugador, EVT_AVANZAR, msg_a_enviar);
                    
                    usleep(ESPERAUS*500);
                }
            break;
            
            case EVT_FIN:
                sscanf(msg.contenidoMsg, "%d", &id_ganador);
                if(id_ganador< 0){
                    printf("Muchacho %d: Las criaturas ganaron, regreso a la isla\n", id_jugador+1);
                }else{
                    printf("Muchacho %d: El muchacho %d escapo del laberinto\n", id_jugador+1, id_ganador);
                }
                fin= TRUE;
            break;
        }
    }

    pthread_exit((void *)"Listo");

}
