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

    int parametro, aleatorio, acierto1=0, acierto2=0, acierto=0;
    int equipo;
    char buffer[TAMANO_BUFFER];
    Equipo e;

    if(argc != 2){
		printf("\nDebe ingresar el numero de equipo(1 o 2).\nEjemplo: ./equipo 1\n");
		return 0;
	}

    parametro=atoi(argv[1]);
    while(parametro!=1 && parametro!=2){
        printf("\nEl numero de equipo debe ser 1 o 2.\n\nEjemplo: ./equipo 1\n\nIngrese nuevamente: ");
        scanf("%d", &parametro);
    }

    printf("\nBIENVENIDO AL PARTIDO DE FUTBOL\nMUCHA SUERTE A AMBOS EQUIPOS\n");

    id_semaforo= creo_semaforo();
    inicia_semaforo(id_semaforo, VERDE);

    /* inicializar aleatorio aca si es necesario*/
    inicializarAleatorio();

    /* limpio el archivo al iniciar */
    abrirArchivo(ARCHIVO, "w");
    escribirArchivo("");
    cerrarArchivo();

    if(parametro== 1){
        abrirArchivo("turnos.dat", "w");
        escribirArchivo("1"); /* por defecto empieza equipo 1 */
        cerrarArchivo();
    }

    memset(linea, 0x00, sizeof(linea));
    memset(buffer, 0x00, sizeof(buffer));

    while(1){
        espera_semaforo(id_semaforo);
        
        /* leo turno actual */
        if(abrirArchivo("turnos.dat", "r")== 0){
            leerArchivoPalabra(buffer);
            equipo= atoi(buffer);
            cerrarArchivo();
        }

        if(parametro== equipo && (acierto1<3 && acierto2<3)){ 

            aleatorio= generarAleatorio(1, 3);
            printf("Ingrese un numero del 1 al 3: ");
            scanf("%d",&e.numero);
            while(e.numero<1 || e.numero>3){
                printf("Numero invalido. Ingrese un numero del 1 al 3: ");
                scanf("%d",&e.numero);
            }
            if(e.numero== aleatorio){
                printf("GOL!!!\n\n");
                if(parametro== 1){
                    acierto1++;
                    acierto= acierto1;
                }else{
                    acierto2++;
                    acierto= acierto2;
                }
            }else{
                printf("Fuera!!!\n");
            }

            abrirArchivo(ARCHIVO, "a");
            if(acierto==    3){
                sprintf(linea, "\nEquipo %d \nNumero a adivinar: %d\nNumero elegido: %d\nCantidad de aciertos: %d\n\n\nEQUIPO GANADOR: %d", parametro, aleatorio, e.numero, acierto, parametro);
                escribirArchivo(linea);
                printf("EL EQUIPO %d GANO EL PARTIDO!!!\n\n", parametro);
            }else{
                sprintf(linea, "\nEquipo %d \nNumero a adivinar: %d\nNumero elegido: %d\nCantidad de aciertos: %d", parametro, aleatorio, e.numero, acierto);
                escribirArchivo(linea);
            }
            cerrarArchivo();

            equipo++; 
            if(equipo>2){
                equipo= 1;
            }
            
            /* cedo el mando al otro equipo */
            abrirArchivo("turnos.dat", "w");
            sprintf(buffer, "%d", equipo);
            escribirArchivo(buffer);
            cerrarArchivo();

            memset(linea, 0x00, sizeof(linea));
            memset(buffer, 0x00, sizeof(buffer));
        }
            levanta_semaforo(id_semaforo);
            usleep(ESPERAUS*1000);
    }


    return 0;
}
