#ifndef _DEF_H
#define _DEF_H

#define TAMANO_BUFFER 256

#define ARCHIVO "pedidos.dat"

#define CERO 0
#define UNO 1

#define FALSE 0
#define TRUE 1

#define ROJO 0
#define VERDE 1

#define PATHKEY "/bin/ls"
#define IDKEY 33

#define ESPERAUS 500 

/*typedef struct{
	int vuelo;
    char destino[20];
    char pasajero[20];
}Avion;

typedef struct{
    int importe;
    int modo_pago;
    int cant_deposito;
    int num_caja;
}Cajero;

typedef struct{
    int num_panel;
    char msg[TAMANO_BUFFER];
}Panel;

typedef struct{
    char color[TAMANO_BUFFER];
    char descripcion[TAMANO_BUFFER];
    int cantidad;
}Medicamento;*/

typedef struct{
    char tipo[TAMANO_BUFFER];
    int postre;
    int precio;
}Comida;

#endif
