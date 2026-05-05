#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "archivos.h"
#include "semaforo.h"
#include "funciones.h"
#include "def.h"
#include "global.h"

int main(int argc,char * argv[]){

    printf("\nPANEL DE INFORMACION\n");

    id_semaforo= creo_semaforo();

    while(1){
        espera_semaforo(id_semaforo);
        if(abrirArchivo(ARCHIVO, "r")==0){
            leerArchivo();
            cerrarArchivo();
        }
        levanta_semaforo(id_semaforo);
        usleep(ESPERAUS*1000); 
    }
    return 0;
}
