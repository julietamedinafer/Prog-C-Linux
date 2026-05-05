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
La empresa se llama “AlquiCor”, y permite la carga de propiedades. Actualmente está manejando 4 propiedades. 
Las propiedades cuestan según el metro cuadrado, que esta en dólares. El mt2 cuesta 2000U$D.

Se debe poder ingresar por el sistema de alquicor, el numero de propiedad identificador
y el nombre de la propiedad. La cantidad de metros cuadrados se calcula de manera aleatoria y va de 20mt2 a 100mt2. 
La empresa debe poder marcar la propiedad reservada y mostrar quien la reservo.
También, los usuarios deben poder ver el estado de las propiedades.*/


int main(int argc,char * argv[]){

    int i;
    Propiedad p;

    printf("\nCENTRO DE CONTROL\n");

    id_semaforo = creo_semaforo();
    inicia_semaforo(id_semaforo, VERDE);

    /* inicializar aleatorio aca si es necesario*/
    inicializarAleatorio();

    /* limpio el archivo al iniciar */
    abrirArchivo(ARCHIVO, "w");
    escribirArchivo("");
    cerrarArchivo();

    memset(linea, 0x00, sizeof(linea));
    memset(p.nombre, 0x00, sizeof(p.nombre));

    espera_semaforo(id_semaforo);
    for(i=1; i<=4; i++){
        printf("\nIngrese el ID de la propiedad %d: ", i);
        scanf("%d",&p.id);
        while (getchar() != '\n'); /* limpio buffer */
        printf("Ingrese el nombre de la propiedad: ");
        fgets(p.nombre, sizeof(p.nombre), stdin); 
        p.nombre[strcspn(p.nombre, "\n")] = 0; /* elimino el salto de linea */
        p.tamano= generarAleatorio(20, 100);
        p.precio= p.tamano * METRO_CUADRADO;
        p.estado= CERO; /* disponible */
        abrirArchivo(ARCHIVO, "a");
        sprintf(linea, "ID: %d, Nombre: %s, Tamano: %d mt2, Precio: %d U$D, Estado: Disponible\n", p.id, p.nombre, p.tamano, p.precio);
        escribirArchivo(linea);
        cerrarArchivo();

        memset(linea, 0x00, sizeof(linea));
        memset(p.nombre, 0x00, sizeof(p.nombre));
    }
    levanta_semaforo(id_semaforo);
    usleep(ESPERAUS*1000);

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
