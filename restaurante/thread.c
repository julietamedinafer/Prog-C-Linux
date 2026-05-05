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
Se cuenta con 3 meseros. Existe un menu con 3 tipos de comidas. 
Según lo que le diga el comenzal por teclado al mesero ell mesero informa el pedido que 
elijio el comenzal, y el restaurante contesta cuando esta listo. La elección del comenzal 
es aleatoria entre los 3 menues, cada 500ms. O sea, cada 500ms, se envia una orden del mesero
al restoderivdor. Cada comida tiene su precio el cual se debe informar, junto con la comida 
(se puede ingresar por teclado, o fijo).

Antes de enviar el mensaje de la comida, se debe preguntar por teclado si quiere postre, 
informando al resto por SI o por NO quiere postre
*/

void *ThreadJugador(void *parametro){
    
    int id_mesero, seleccion;
    Comida c;
    char msg_a_enviar[TAMANO_BUFFER];
    int fin= FALSE;
    mensaje msg; 
    DatosMesero *datos;

    /* variables locales del thread con datos del proceso principal */
    datos= (DatosMesero*)parametro; /* puntero a la estructura de los datos */
    id_mesero= datos->id_mesero;

    /* limpieza */
    memset(msg_a_enviar, 0x00, sizeof(msg_a_enviar));
    
    while(fin== FALSE){
        recibir_mensaje(id_cola, MSG_MESERO+id_mesero, &msg);

        switch(msg.evento) {
            case EVT_INICIO:
                printf("\n\nSoy el mesero %d\n", id_mesero+1);
                pthread_mutex_lock(&mutex);

                printf("\nMenu:\n1) Pizza\n2) Hamburguesa\n3) Pancho\n");
                seleccion= obtener_aleatorio(1, 3);
                printf("\nMenu %d seleccionado\n", seleccion);

                if(seleccion== 1){
                    strcpy(c.descripcion, "Pizza");
                    c.precio= 22000;
                }else if(seleccion== 2){
                    strcpy(c.descripcion, "Hamburguesa");
                    c.precio= 15000;
                }else if(seleccion== 3){
                    strcpy(c.descripcion, "Pancho");
                    c.precio= 3000;
                }
                printf("\nPrecio: %d\n", c.precio);

                printf("\n¿Desea postre ($800)?\n");
                c.postre= obtener_aleatorio(0, 1); /* 1= si, 0= no */
                if(c.postre== 1){
                    c.precio+= 800;
                printf("\nPostre seleccionado\nPrecio final: %d\n", c.precio);
                }else{
                    printf("\nNo se selecciono postre\nPrecio final: %d\n", c.precio);
                }

                memset(msg_a_enviar, 0x00, sizeof(msg_a_enviar));
                sprintf(msg_a_enviar, "%d %s %d", c.precio, c.descripcion, c.postre);

                enviar_mensaje(id_cola, MSG_RESTO, MSG_MESERO+id_mesero, EVT_PREPARAR, msg_a_enviar);
                pthread_mutex_unlock(&mutex);
                
                usleep(ESPERAUS*1000);
            break;
            
            case EVT_LISTO:
                printf("Mesero %d: Pedido listo\n", id_mesero+1);
            break;
            case EVT_FIN:
                printf("Mesero %d: Finalizando turno\n", id_mesero+1);
                enviar_mensaje(id_cola, MSG_RESTO, MSG_MESERO+id_mesero, EVT_FIN, "");
                fin = TRUE;
            break;
        }
    }

    pthread_exit((void *)"Listo");

}
