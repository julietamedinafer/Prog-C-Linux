#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "archivos.h"
#include "semaforo.h"
#include "funciones.h"
#include "def.h"
#include "global.h"

/*CONSIGNA:
Cada sistema de control debe mostrar la totalidad de menues realizados, por cada sistema (A, B, ó C) y sus precios.

Cada 100ms cada sistema debe chequear si tiene nuevos menues a realizar.
*/

int main(int argc,char * argv[]){
    int parametro;    

    if(argc != 2){
		printf("\nDebe ingresar el numero de sistema(1, 2 o 3).\nEjemplo: ./comida 1\n");
		return 0;
	}else{
        parametro=atoi(argv[1]);
        while(parametro< 1 || parametro> 3){
            printf("\nEl numero de sistema debe ser 1, 2 o 3.\n\nEjemplo: ./comida 1\n\nIngrese nuevamente: ");
            scanf("%d", &parametro);
        }
        printf("BIENVENIDO AL SISTEMA\n");
    }
       
    id_semaforo = creo_semaforo();

    while(1){
        espera_semaforo(id_semaforo);
        if(abrirArchivo(ARCHIVO, "r")==0){
            leerMensajePropio(parametro);
            cerrarArchivo();
        }
        levanta_semaforo(id_semaforo);
        usleep(ESPERAUS*1000); 
    }
    return 0;
}
