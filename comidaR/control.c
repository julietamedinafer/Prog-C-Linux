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
En este caso se realizarán 3 tipos de ordenes que se brindan en el menú ejecutivo, y dependen del precio, por lo que esta el Menu A ($1000), Menú B ($2000), y Menú C ($3000). 
O sea, aleatoriamente se elije el tipo de menú a realizar. Cada menú puede contener o no postre (se incrementa el precio $200).

Se deriva según el menú que salió, al sistema de control, siendo el sistema A (menú A), el sistema B (menú B) y el sistema C (menú C) donde se prepara el pedido. 
Por lo que se debe luego de elegir el tipo de menú, solicitar si va a querer o no postre (agregar un menu que permita el ingreso por teclado si quiere o no postre).
*/


int main(int argc,char * argv[]){

    int sistema;
    int contA= 0, contB= 0, contC= 0, precioA= 0, precioB= 0, precioC= 0;
    Comida c;

    printf("\nCENTRO DE CONTROL\n");

    id_semaforo = creo_semaforo();
    inicia_semaforo(id_semaforo, VERDE);

    /* limpio el archivo al iniciar */
    abrirArchivo(ARCHIVO, "w");
    escribirArchivo("");
    cerrarArchivo();

    /* inicializar aleatorio aca si es necesario*/
    inicializarAleatorio();

    memset(linea, 0x00, sizeof(linea));
    memset(c.tipo, 0x00, sizeof(c.tipo));
    c.precio= 0;

    while(1){
        printf("1) Menu A, precio: 1000\n2) Menu B, precio: 2000\n3) Menu C, precio: 3000\n");
        sistema= generarAleatorio(1, 3);
        printf("\nMenu elegido: %d\n", sistema);

        printf("\nDesea postre?\n1) Si\n2) No\nElija una opcion: ");
        scanf("%d",&c.postre);
        while(c.postre!= 1 && c.postre!= 2){
            printf("Opcion invalida. Desea postre?\n1) Si\n2) No\nElija una opcion: ");
            scanf("%d",&c.postre);
        }
        while (getchar() != '\n'); /* limpiar buffer */


        if(sistema== 1){
            strcpy(c.tipo, "Menu A");
            contA++;
            c.precio= 1000;
            if(c.postre == 1){
            c.precio+= 200;
            }
            precioA+= c.precio;

            espera_semaforo(id_semaforo);
            abrirArchivo(ARCHIVO, "w");
            sprintf(linea, "%d Tipo menu: %s, Total vendidos: %d, Monto recaudado: %d", sistema, c.tipo, contA, precioA);
            escribirArchivo(linea);
            cerrarArchivo();
            levanta_semaforo(id_semaforo);
            usleep(ESPERAUS*1000);

        }else if(sistema== 2){
            strcpy(c.tipo, "Menu B");
            contB++;
            c.precio= 2000;
            if(c.postre == 1){
            c.precio+= 200;
            }
            precioB+= c.precio;

            espera_semaforo(id_semaforo);
            abrirArchivo(ARCHIVO, "w");
            sprintf(linea, "%d Tipo menu: %s, Total vendidos: %d, Monto recaudado: %d", sistema, c.tipo, contB, precioB);
            escribirArchivo(linea);
            cerrarArchivo();
            levanta_semaforo(id_semaforo);
            usleep(ESPERAUS*1000);
        }else if(sistema== 3){
            strcpy(c.tipo, "Menu C");
            contC++;
            c.precio= 3000;
            if(c.postre == 1){
            c.precio+= 200;
            }
            precioC+= c.precio;

            espera_semaforo(id_semaforo);
            abrirArchivo(ARCHIVO, "w");
            sprintf(linea, "%d Tipo menu: %s, Total vendidos: %d, Monto recaudado: %d", sistema, c.tipo, contC, precioC);
            escribirArchivo(linea);
            cerrarArchivo();
            levanta_semaforo(id_semaforo);
            usleep(ESPERAUS*1000);
        }else if(sistema== 0){
            printf("\nFin del programa\n");
            return 0;
        }

        memset(linea, 0x00, sizeof(linea));
        memset(c.tipo, 0x00, sizeof(c.tipo));
    }
    return 0;
}
