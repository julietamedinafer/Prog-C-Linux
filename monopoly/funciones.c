#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "funciones.h"

void inicializar_aleatorio(void){
	srand(time(NULL));
}

int obtener_aleatorio(int desde, int hasta){
	return (rand()%(hasta-desde+1))+desde;
}

int aplicar_posicion(int posicion, int saldo, int id_jugador){
    int nuevo_saldo= saldo;
    
    switch(posicion){
        case 3:
            nuevo_saldo-= 5;
            printf("jugador %d: compra caramelos, pierde $5\n", id_jugador+1);
            break;
        case 4:
            nuevo_saldo-= 100;
            printf("jugador %d: cuota de la universidad, pierde $100\n", id_jugador+1);
            break;
        case 6:
            nuevo_saldo-= 200;
            printf("jugador %d: impuestos, pierde $200\n", id_jugador+1);
            break;
        case 7:
            nuevo_saldo+= 150;
            printf("jugador %d: cumple anos, recibe $150\n", id_jugador+1);
            break;
        case 9:
            nuevo_saldo-= 40;
            printf("jugador %d: paga la jubilacion, pierde $40\n", id_jugador+1);
            break;
        case 14:
            printf("jugador %d: entra a la carcel, pierde un turno\n", id_jugador+1);
            break;
        case 15:
            nuevo_saldo-= 400;
            printf("jugador %d: choca el auto, pierde $400\n", id_jugador+1);
            break;
        case 18:
            nuevo_saldo+= 300;
            printf("jugador %d: realiza un trabajo, gana $300\n", id_jugador+1);
            break;
        case 19:
            nuevo_saldo-= 600;
            printf("jugador %d: le roban, pierde $600\n", id_jugador+1);
            break;
        case 23:
            nuevo_saldo+= 800;
            printf("jugador %d: gana la loteria, gana $800\n", id_jugador+1);
            break;
        case 25:
            nuevo_saldo+= 75;
            printf("jugador %d: vende un sillon, gana $75\n", id_jugador+1);
            break;
        case 28:
            nuevo_saldo-= 340;
            printf("jugador %d: pierde la billetera, pierde $340\n", id_jugador+1);
            break;
        case 31:
            nuevo_saldo-= 120;
            printf("jugador %d: bitcoin baja, pierde $120\n", id_jugador+1);
            break;
        case 34:
            nuevo_saldo+= 120;
            printf("jugador %d: bitcoin sube, gana $120\n", id_jugador+1);
            break;
        case 37:
            nuevo_saldo-= 1000;
            printf("jugador %d: se compra un auto, pierde $1000\n", id_jugador+1);
            break;
        case 39:
            nuevo_saldo+= 500;
            printf("jugador %d: vende el auto, gana $500\n", id_jugador+1);
            break;
        case 45:
            nuevo_saldo-= 400;
            printf("jugador %d: se casa, pierde $400\n", id_jugador+1);
            break;
        case 48:
            nuevo_saldo-= 15;
            printf("jugador %d: compra comida, pierde $15\n", id_jugador+1);
            break;
        case 51:
            nuevo_saldo+= 50;
            printf("jugador %d: vende la tele, gana $50\n", id_jugador+1);
            break;
        case 53:
            printf("jugador %d: pierde un turno\n", id_jugador+1);
            break;
        case 54:
            nuevo_saldo+= 300;
            printf("jugador %d: hace otro trabajo, gana $300\n", id_jugador+1);
            break;
        case 55:
            nuevo_saldo+= 100;
            printf("jugador %d: recibe $100\n", id_jugador+1);
            break;
    }
    
    return nuevo_saldo;
}
