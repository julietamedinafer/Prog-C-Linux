#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "archivos.h"
#include "semaforo.h"
#include "funciones.h"
#include "def.h"
#include "global.h"
 
/* CONSIGNA:
En una entidad de productos farmacologicos, se destina por medio de un sistema de control (llamado "tu medicamento") la realización y la derivación de cada medicamento. En este caso se realizarán 3 medicamentos aleatoriamente, y cada tipo de medicamento se separa en tandas de: 5, 10 y 20 pastillas por cada medicamento. 
O sea, aleatoriamente se elije el tipo de medicamento a realizar, y su color de la capsula, de 1 a 3, pudiendo repetirse los colores (siendo 1 Rojo, 2 Amarillo, 3 Fucsia). 

Independientemente del medicamento, se deriva según su cantidad al sistema de control, siendo el sistema A (5 pastillas), el sistema B (10 pastillas) y el sistema C (20 pastillas) donde se empaquetan las pastillas. 
Por lo que se debe luego del tipo de medicamento, realizar un menu que permita el ingreso por teclado de la cantidad de pastillas a realizar (5, 10 ó 20).
*/


int main(int argc,char * argv[]){

    int tipo, sistema;
    Medicamento m;

    printf("\nCENTRO DE CONTROL\n");

    id_semaforo = creo_semaforo();
    inicia_semaforo(id_semaforo, VERDE);

    /* inicializar aleatorio aca si es necesario*/
    inicializarAleatorio();

    memset(linea, 0x00, sizeof(linea));
    memset(m.color, 0x00, sizeof(m.color));
    memset(m.descripcion, 0x00, sizeof(m.descripcion));

    while(1){
        printf("Tipos de medicamentos:\n1) Rojo\n2) Amarillo\n3) Fucsia\n");
        tipo= generarAleatorio(1, 3); 
        
        if(tipo == 1){
            strcpy(m.color, "Rojo");
        }else if(tipo == 2){
            strcpy(m.color, "Amarillo");
        }else if(tipo == 3){
            strcpy(m.color, "Fucsia");
        }

        printf("Ingrese cantidad de pastillas (5, 10 o 20): ");
        scanf("%d",&m.cantidad);
        while(m.cantidad != 5 && m.cantidad != 10 && m.cantidad != 20){
            printf("Cantidad invalida. Ingrese cantidad de pastillas (5, 10 o 20): ");
            scanf("%d",&m.cantidad);
        }
        while (getchar() != '\n'); /* Limpiar buffer */
        printf("Ingrese descripcion del medicamento: ");
        fgets(m.descripcion, sizeof(m.descripcion), stdin);
        m.descripcion[strcspn(m.descripcion, "\n")] = 0; /* elimina el salto de linea */
        
       if(m.cantidad == 5){
            sistema= 1;
            espera_semaforo(id_semaforo);
            abrirArchivo(ARCHIVO, "w");
            
            sprintf(linea, "%d Cantidad de pastillas: %d, Tipo de medicamento: %s, Descripcion: %s", sistema, m.cantidad, m.color, m.descripcion);
            escribirArchivo(linea);
            cerrarArchivo();
            levanta_semaforo(id_semaforo);
            usleep(ESPERAUS*1000);
        }else if(m.cantidad == 10){
            sistema= 2;
            espera_semaforo(id_semaforo);
            abrirArchivo(ARCHIVO, "w");
            
            sprintf(linea, "%d Cantidad de pastillas: %d, Tipo de medicamento: %s, Descripcion: %s", sistema, m.cantidad, m.color, m.descripcion);
            escribirArchivo(linea);
            cerrarArchivo();
            levanta_semaforo(id_semaforo);
            usleep(ESPERAUS*1000);
        }else if(m.cantidad == 20){
            sistema= 3;
            espera_semaforo(id_semaforo);
            abrirArchivo(ARCHIVO, "w");

            sprintf(linea, "%d Cantidad de pastillas: %d, Tipo de medicamento: %s, Descripcion: %s", sistema, m.cantidad, m.color, m.descripcion);
            escribirArchivo(linea);
            cerrarArchivo();
            levanta_semaforo(id_semaforo);
            usleep(ESPERAUS*1000);
        }

        memset(linea, 0x00, sizeof(linea));
        memset(m.color, 0x00, sizeof(m.color));
        memset(m.descripcion, 0x00, sizeof(m.descripcion));
    }
    return 0;
}
