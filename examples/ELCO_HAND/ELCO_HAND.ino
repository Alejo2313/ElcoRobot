
#include <Servo.h>
#include "tras.h"

#define SERVOS 5
#define MOVE  4

Servo servos[5];

static uint8_t servoPin[SERVOS]= {9,8,6,12,4};

static uint8_t defaultMove[MOVE][SERVOS] = {{90,180,90,45, 0},{90,90,90,45,0}, {0,180, 90,45,90}, {90,90,90,90,90}};

static uint8_t homeMove[SERVOS] ={90,90,90,90,90};

uint8_t data[6];
void setup() {
  for(int i= 0; i < SERVOS; i++){
      servos[i].attach(servoPin[i]);
      servos[i].write(90);
      delay(100);
    }
  Serial.begin(9600);
  /*Inicializa el trasmisor i2c*/
  tras_begin(0x03, 10);
}

void loop() {
   if(data_available()){  //Devuelve si hay datos disponibles
    readData(data);
    for(int i = 0; i < MOVE; i++){
      for(int c = 0; c < SERVOS; c++)
        servos[c].write(defaultMove[i][c]);
        delay(1000);
      }
    }
}
