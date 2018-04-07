#include <Servo.h>

/******************************************************************/
/*                ElcoRobo SLAVE                                  */
/*  Ejemplo de funcionamiento de las funciones en modo SLAVE      */
/*                                                                */
/*  @author  ->  Alejo                                            */
/*  @date    ->  03/11/18                                         */
/*  @version ->  1.0                                              */
/******************************************************************/




#include "tras.h"

uint8_t data[6];

Servo servoMotor;
void setup()
{
   servoMotor.attach(11);
  Serial.begin(9600);
  /*Inicializa el trasmisor i2c*/
  tras_begin(0x02, 3);      // El primer parametro es la direccion del dispositivo
                            // El segundo es el pin de control
}
void loop()
{
  if(data_available()){  //Devuelve si hay datos disponibles
    readData(data);      //Devuelve los datos en el buffer. El parametro es el  array donde se almacenan los datos.
    Serial.println((int)INPUT_BUFFER[0]);
    servoMotor.write(INPUT_BUFFER[0]);
  // Esperamos 1 segundo
  delay(1000);
  }
  
}
