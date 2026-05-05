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
Cada sistema de control debe mostrar la totalidad de medicamentos y pastillas realizadas,
por cada sistema (A, B, ó C).*/

int main(int argc,char * argv[]){
    int parametro;    

    if(argc != 2){
		printf("\nParametro inválido.\nDebe ingresar el numero de sistema(1, 2 o 3).\nEjemplo: ./medicamento 1\n");
		return 0;
	}else{
        parametro=atoi(argv[1]);
        printf("BIENVENIDO AL SISTEMA DEL MEDICAMENTO\n");
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
