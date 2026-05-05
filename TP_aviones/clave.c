#include "clave.h"
#include "definiciones.h"
#include "stdio.h"
#include "stdlib.h"

key_t creo_clave() 
{

key_t clave;
clave = ftok (PROGRAMA, PARAM); /*(char*,int)*/
if (clave == (key_t)-1)
{
printf("No puedo conseguir clave semáforo, mem compartida, etc.\n");
exit(0);
}
return clave;
}

