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

#define ESPERAUS 800 

#define CANTIDAD 5

/* ESTRUCTURAS SEGUNDA PARTE DE LA MATERIA*/

/*typedef struct{
    int numero_pensado;
    int estado_acierto;
    char nombre_ganador[TAMANO_BUFFER];
}dato;

typedef struct{
    int codigo;
    int cantidad;
    char nombre[TAMANO_BUFFER];
}dato; */

/*-------------  EJERCICIO BANCO CAJERO  -------------*/

/* #define TIPO_BANCO 1L  xq es un long 
#define DESDE_CLIENTE 1
#define HASTA_CLIENTE 100
#define DESDE_SALDO 1
#define HASTA_SALDO 100000

typedef enum{
	CONSULTA_SALDO,
	DEPOSITO,
	EXTRACCION,
	RTA_SALDO,
	RTA_SALDO_E,
	RTA_DEPOSITO,
	RTA_DEPOSITO_E,
	RTA_EXTRACCION,
	RTA_EXTRACCION_E,
	FIN
}EventType;

typedef struct{
	int codigo;
	int saldo;
}Cliente; */

/*-------------  EJERCICIO RULETA RUSA  -------------*/

#define MSG_NADIE 0
#define MSG_REVOLVER 1
#define MSG_JUGADOR 2 /* + numero de jugador */

typedef enum{
	EVT_NINGUNO,
	EVT_INICIO,
	EVT_DISPARO,
	EVT_SALVADO,
	EVT_FIN
}EventType;

typedef struct{
	int num_jugador;
	int id_cola_mensaje;
	int *vector_tambor;
	int fue_eliminado;
}DatosJugador;

#endif
