#include "stdlib.h"
#include "time.h"
#include "stdio.h"
#include "funciones.h"
 
/*   GENERAR ALEATORIOS   */

void inicializarAleatorio(void){
	srand(time(NULL));
}

int generarAleatorio(int desde, int hasta){
	return (rand()%( hasta - desde + 1)) + desde;
}

/*   MENU   */

/*void menu(int op,char *mensaje[]){
    int i;
    printf("\n********** Bienvenido al programa **********\n");
    printf("\n Elija una opcion: \n");
    printf("0 para salir");
    for(i=0;i<op;i++){
        printf("\n%d %s",i+1,mensaje[i]);    
    }
    printf("\n");
    
    
}*/

/*   FINALIZAR PROGRAMA  */
