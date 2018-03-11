/******************************************************************/
/*                ElcoRobo SLAVE                                  */
/*  Ejemplo de funcionamiento de las funciones en modo SLAVE      */
/*                                                                */
/*  @author  ->  Alejo                                            */
/*  @date    ->  03/11/18                                         */
/*  @version ->  1.0                                              */
/******************************************************************/




#include <VirtualWire.h>
#include "tras.h"

uint8_t t = 400; 
uint8_t data[6];
uint8_t pinzumbador = 9;

void setup()
{
  Serial.begin(9600);
  /*Inicializa el trasmisor i2c*/
  tras_begin(0x01, 3);      // El primer parametro es la direccion del dispositivo
                            // El segundo es el pin de control
}
void loop()
{
  if(data_available()){  //Devuelve si hay datos disponibles
    readData(data);      //Devuelve los datos en el buffer. El parametro es el  array donde se almacenan los datos.
    Serial.println(t, data[0]);
  }
  
}
