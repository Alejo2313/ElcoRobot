#include "tras.h"
#include <Servo.h>

uint8_t data[6];

Servo servoMotor;
void setup()
{
   servoMotor.attach(2);
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
    servoMotor.write(data[0]);
  delay(100);
  }
  
}
