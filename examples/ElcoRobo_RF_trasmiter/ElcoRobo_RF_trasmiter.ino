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
uint8_t data[6]= {0,0,0,0,0,0};
void loop()
{

      if(Serial.available()){
        uint8_t tmp = Serial.read();

        switch(tmp){
          case 'a':
            data[0] = 0;
            rf_sendData(0x02,DATA_IN, data);
            break;
         case 'b':
            data[0] = 45;
            rf_sendData(0x02,DATA_IN, data);
            break;
         case 'c':
            data[0] = 90;
            rf_sendData(0x02,DATA_IN, data);
            break;
         case 'd':
            data[0] = 135;
            rf_sendData(0x02,DATA_IN, data);
            break;
          case 'e':
            data[0] = 180;
            rf_sendData(0x02,DATA_IN, data);
            break;
          case 'x':
            data[0] = 45;
            rf_sendData(0x03,DATA_IN, data);
            break;  
          case 'u':
            data[0] = 2;
            rf_sendData(0x04,DATA_IN, data);
            break;
          case 't':
            data[0] = 1;
            rf_sendData(0x04,DATA_IN, data);
            break;
          }

      }
     // delay(100);
      //el primer parametro es la direccion a la que van los datos.
                                        //el segundo parametro es el comando. de mometo hay 2, DATA_IN-> Envia datos AUTH->(en desarrollo)
                                        //el tercero son los datos, DEBE SER UNA ARRAY DE AL MENOS 6 ELEMENTOS
}
