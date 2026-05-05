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
habrá interesados (de 1 a n), los cuales realizan consultas a “AlquiCor”
-Nombre de la propiedad
-Valor de la propiedad
-Si está reservada o no
-Metraje de la propiedad
Ahora, si un interesado quiere reservar la propiedad, le debe enviar a la empresa, el numero
o algún dato identificativo de la propiedad que quiere reservar, junto con su apellido y su dni. 
*/

int main(int argc,char * argv[]){
    /*int parametro;*/
    int opcion;
    Interesado in;

    if(argc != 2){
		printf("\nDebe ingresar el numero de interesado(1, 2 o 3).\nEjemplo: ./interesado 1\n");
		return 0;
	}else{
        /*parametro=atoi(argv[1]);*/
        printf("\nBIENVENIDO A ALQUICOR\n\nCargando propiedades...");
    }
       
    id_semaforo = creo_semaforo();

    memset(linea, 0x00, sizeof(linea));
    memset(in.apellido, 0x00, sizeof(in.apellido));

    espera_semaforo(id_semaforo);
    if(abrirArchivo(ARCHIVO, "r")==0){
        leerArchivo();
         cerrarArchivo();
    }
    levanta_semaforo(id_semaforo);
    usleep(ESPERAUS*1000); 

    printf("\nDesea reservar alguna propiedad?\n1) si\n2) no\nElija una opcion: ");
    scanf("%d",&opcion);
    while(opcion!= 1 && opcion!= 2){
        printf("Opcion invalida. Desea reservar alguna propiedad?\n1) si\n2) no\nElija una opcion: ");
        scanf("%d",&opcion);
    }

    if(opcion==1){
        printf("\nIngrese el ID de la propiedad que desea reservar: ");
        scanf("%d",&in.id_propiedad);
        while (getchar() != '\n'); /* limpiar buffer */
            
        printf("Ingrese su apellido: ");
        fgets(in.apellido, sizeof(in.apellido), stdin);
        in.apellido[strcspn(in.apellido, "\n")] = 0; /* elimino el salto de linea */

        printf("Ingrese su DNI: ");
        scanf("%d",&in.dni);
        while (getchar() != '\n'); /* limpiar buffer */

        espera_semaforo(id_semaforo);
        abrirArchivo(ARCHIVO, "a");
        sprintf(linea, "\nLa propiedad con ID %d fue reservada por %s, DNI: %d\n", in.id_propiedad, in.apellido, in.dni);
        escribirArchivo(linea);
        cerrarArchivo();
        levanta_semaforo(id_semaforo);
        usleep(ESPERAUS*1000);
    }

    printf("\nPropiedad reservada con exito. Gracias por utilizar AlquiCor!\n");
    

    return 0;
}
