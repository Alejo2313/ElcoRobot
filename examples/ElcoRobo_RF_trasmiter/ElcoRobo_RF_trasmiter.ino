/******************************************************************/
/*                ElcoRobo RF_EMITER                              */
/*  Ejemplo de funcionamiento de las funciones del trasmisor      */
/*                                                                */
/*  @author  ->  Alejo                                            */
/*  @date    ->  03/11/18                                         */
/*  @version ->  1.0                                              */
/******************************************************************/





#include "tras.h"
 
void setup()
{
  Serial.begin(9600);
  /* Se inicializa el trasmisor de RF*/
  tras_rf_begin(4, RF_EMITER);// el primer parametro es el pin de datos del RF
                              // El segunto parametro es el modo de funcionamiento. Hay dos opciones RF_EMITER y RF_RECEPTOR
}

void loop()
{
    if (Serial.available()) 
    {
      int m = Serial.read();
      data[0]=data[0]+100; 
      /*Se envian los datos a travez del RF*/
      rf_sendData(0x01,DATA_IN, data);  //el primer parametro es la direccion a la que van los datos.
                                        //el segundo parametro es el comando. de mometo hay 2, DATA_IN-> Envia datos AUTH->(en desarrollo)
                                        //el tercero son los datos, DEBE SER UNA ARRAY DE AL MENOS 6 ELEMENTOS
    }
}
