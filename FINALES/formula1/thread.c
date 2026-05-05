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
Es una carrera en la que Alpine, Mclaren y Ferrari son los más rápidos.
Cada formula 1, avanza de 5 a 10 metros por 0,5segundos, pero si son Alpine, Ferrari
o Mclaren, avanzan de 6 a 12mt por 0,5segundos. Y cada vez que avanza, aleatoriamente
debe chequear si choca o se sale de la pista (de 1 a 2, si sale 2 , sale de la pista y
pierde la carrera). También, cuando supera los 50mt 100mt, 150mt, 200 mt y 250mt debe
chequear si debe ir a boxes (entre 1 y 3, si sale 1 debe ir a boxes, y se pierde una
vuelta, si sale 2 o 3 sigue corriendo).

La carrera termina con el primer auto que llega a 295mt. Si llega más de un auto a
superar los 295mt, el ganador es el que más puntos tiene y si hay con mismo valor en
el mismo turno, es empate.
*/

void *ThreadJugador(void *parametro){
    
    int id_auto, posicion, puntos, cont_avance, meta, cant_participantes, accidente, cont_boxes=0, boxes;
    int i, id_ganador, participantes_activos= 0;
    char msg_a_enviar[TAMANO_BUFFER];
    int fin= FALSE;
    mensaje msg; 
    DatosParticipante *datos;

    /* variables locales del thread con datos del proceso principal */
    datos= (DatosParticipante*)parametro; /* puntero a la estructura de los datos */
    id_auto= datos->id_auto;
    posicion= datos->posicion;
    puntos= datos->puntos;
    cont_avance= datos->cont_avance;

    /* limpieza */
    memset(msg_a_enviar, 0x00, sizeof(msg_a_enviar));
    
    while(fin== FALSE){
        recibir_mensaje(id_cola, MSG_PARTICIPANTE+id_auto, &msg);

        switch(msg.evento){
            case EVT_INICIO:
                sscanf(msg.contenidoMsg, "%d %d", &meta, &cant_participantes); 
                participantes_activos= cant_participantes;
                pthread_mutex_lock(&mutex);
                
                datos->posicion= posicion;
                datos->puntos= puntos;
                pthread_mutex_unlock(&mutex);
                
                enviar_mensaje(id_cola, MSG_LUGAR, MSG_PARTICIPANTE+id_auto, EVT_NINGUNO, "");
                
                usleep(ESPERAUS*500);
            break;
            
            case EVT_AVANZAR:
                sscanf(msg.contenidoMsg, "%d", &participantes_activos);

                if(posicion!= -1 && participantes_activos>1){
                    accidente= obtener_aleatorio(1, 2);
                    if(accidente== 2){
                        printf("Auto %d: Tuve un accidente y quede fuera de la carrera\n\n", id_auto+1);
                        posicion= -1;
                        memset(msg_a_enviar, 0x00, sizeof(msg_a_enviar));
                        sprintf(msg_a_enviar, "%d %d", posicion, cont_avance);
                        enviar_mensaje(id_cola, MSG_LUGAR, MSG_PARTICIPANTE+id_auto, EVT_INFO_CARRERA, msg_a_enviar);
                    }else{
                        if(id_auto==1 || id_auto==2 || id_auto==3){
                            posicion= obtener_aleatorio(6, 12);
                        }else{
                            posicion= obtener_aleatorio(5, 10);
                        } 

                        cont_avance+= posicion;
                        if((cont_avance>50 && cont_boxes<1) || (cont_avance>100 && cont_boxes<2) || (cont_avance>150 && cont_boxes<3) || (cont_avance>200 && cont_boxes<4) || (cont_avance>250 && cont_boxes<5)){
                            boxes= obtener_aleatorio(1, 3);
                            if(boxes== 1){
                                cont_boxes++;
                                printf("Auto %d: Fui a boxes, pierdo una vuelta\n", id_auto+1);
                                cont_avance-= posicion; /* no avanza en esta vuelta */
                                posicion= -2;
                                memset(msg_a_enviar, 0x00, sizeof(msg_a_enviar));
                                sprintf(msg_a_enviar, "%d %d", posicion, cont_avance); 
                                enviar_mensaje(id_cola, MSG_LUGAR, MSG_PARTICIPANTE+id_auto, EVT_INFO_CARRERA, msg_a_enviar);
                            }else{
                                printf("Auto %d: avance %dm, total recorrido %dm\n\n", id_auto+1, posicion, cont_avance);
                                memset(msg_a_enviar, 0x00, sizeof(msg_a_enviar));
                                sprintf(msg_a_enviar, "%d %d", posicion, cont_avance);
                                enviar_mensaje(id_cola, MSG_LUGAR, MSG_PARTICIPANTE+id_auto, EVT_INFO_CARRERA, msg_a_enviar);
                            }
                        }else{
                            printf("Auto %d: avance %dm, total recorrido %dm\n\n", id_auto+1, posicion, cont_avance);
                            memset(msg_a_enviar, 0x00, sizeof(msg_a_enviar));
                            sprintf(msg_a_enviar, "%d %d", posicion, cont_avance); 
                            enviar_mensaje(id_cola, MSG_LUGAR, MSG_PARTICIPANTE+id_auto, EVT_INFO_CARRERA, msg_a_enviar);
                        }
                        
                        puntos= cont_avance;

                        if(cont_avance>= meta){
                            printf("Auto %d: gane la carrera!\n\n", id_auto+1);
                            
                            /* avisar a todos los otros que perdieron */
                            for(i= 0; i< cant_participantes; i++){
                                if(i!= id_auto){
                                    memset(msg_a_enviar, 0x00, sizeof(msg_a_enviar));
                                    sprintf(msg_a_enviar, "%d %d", id_auto+1, puntos);
                                    enviar_mensaje(id_cola, MSG_PARTICIPANTE+i, MSG_PARTICIPANTE+id_auto, EVT_FIN, msg_a_enviar);
                                }
                            }
                            
                            memset(msg_a_enviar, 0x00, sizeof(msg_a_enviar));
                            sprintf(msg_a_enviar, "%d %d", id_auto+1, puntos);
                            enviar_mensaje(id_cola, MSG_LUGAR, MSG_PARTICIPANTE+id_auto, EVT_GANADOR, msg_a_enviar);
                            
                            fin= TRUE;
                        }
                    }
                    
                    /* acceso a recursos compartidos */
                    pthread_mutex_lock(&mutex);
                    datos->posicion= posicion;
                    datos->puntos= puntos;
                    datos->cont_avance= cont_avance;
                    pthread_mutex_unlock(&mutex);
                    
                    usleep(ESPERAUS*500);
                }else if(posicion== -1){
                    memset(msg_a_enviar, 0x00, sizeof(msg_a_enviar));
                    sprintf(msg_a_enviar, "%d %d", posicion, cont_avance);
                    enviar_mensaje(id_cola, MSG_LUGAR, MSG_PARTICIPANTE+id_auto, EVT_INFO_CARRERA, msg_a_enviar);
                }else if(posicion== -2){
                    memset(msg_a_enviar, 0x00, sizeof(msg_a_enviar));
                    sprintf(msg_a_enviar, "%d %d", posicion, cont_avance);
                    enviar_mensaje(id_cola, MSG_LUGAR, MSG_PARTICIPANTE+id_auto, EVT_INFO_CARRERA, msg_a_enviar);
                }else if(participantes_activos<=1){
                    printf("Auto %d: gane la carrera!\n", id_auto+1);
                            
                            /* avisar a todos los otros que perdieron */
                            for(i= 0; i< cant_participantes; i++){
                                if(i!= id_auto){
                                    memset(msg_a_enviar, 0x00, sizeof(msg_a_enviar));
                                    sprintf(msg_a_enviar, "%d %d", id_auto+1, puntos);
                                    enviar_mensaje(id_cola, MSG_PARTICIPANTE+i, MSG_PARTICIPANTE+id_auto, EVT_FIN, msg_a_enviar);
                                }
                            }
                            
                            memset(msg_a_enviar, 0x00, sizeof(msg_a_enviar));
                            sprintf(msg_a_enviar, "%d %d", id_auto+1, puntos);
                            enviar_mensaje(id_cola, MSG_LUGAR, MSG_PARTICIPANTE+id_auto, EVT_GANADOR, msg_a_enviar);
                            
                            fin= TRUE;
                }
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
