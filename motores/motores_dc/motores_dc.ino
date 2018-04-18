 /* 
  *  Javier Otero Martinez
  *  Miguel Garcia Maya
  *  ELCO - 2018
  *  
  */
#include "tras.h"

#define IZQ_GIR1 7   //Verde
#define IZQ_GIR0 6   //Amarillo
#define DER_GIR1 4   //Naranja
#define DER_GIR0 5   //Rojo
#define DATA_PIN 10
#define PWM_IZQ  8   //Gris
#define PWM_DER  3   //Blanco 

#define VELOCIDAD_FULL 230
#define VELOCIDAD_GIRO 100

uint8_t data[6];

void para();
void avanza();
void atras();
void gira_derecha();
void gira_izquierda();

void setup() {
 pinMode (PWM_IZQ, OUTPUT); 
 pinMode (PWM_DER, OUTPUT);
 pinMode (IZQ_GIR0, OUTPUT);
 pinMode (IZQ_GIR1, OUTPUT);
 pinMode (DER_GIR0, OUTPUT);
 pinMode (DER_GIR1, OUTPUT);
 pinMode (DATA_PIN, INPUT);

 tras_begin(0x04, DATA_PIN);
 
}


void loop() {
  /*
  avanza();
  delay(1000);
  gira_derecha();
  delay(1000);
  gira_izquierda();
  delay(1000);
  para();
  delay(2000);
  atras();
  delay(2000);
  para();
  while (1) {delay(1000);}
  */
  
  if(data_available()){  //Devuelve si hay datos disponibles
    readData(data);      //Devuelve los datos en el buffer. El parametro es el  array donde se almacenan los datos.
    if (INPUT_BUFFER[0] == 1) {
      para();
      Serial.println("Para");
    } else if (INPUT_BUFFER[0] == 2){
      avanza();
      Serial.println("Avanza");
    } else if (INPUT_BUFFER[0] == 3){
      atras();
    }
    if (data[1] == 2){
      gira_derecha();
    } else if (data[1] == 3) {
      gira_izquierda();
    }
    //Serial.println((int)INPUT_BUFFER[0]);}
  }
}

void avanza(){
 digitalWrite (IZQ_GIR0, HIGH);
 digitalWrite (IZQ_GIR1, LOW);
 digitalWrite (DER_GIR0, HIGH);
 digitalWrite (DER_GIR1, LOW);
 analogWrite (PWM_IZQ, VELOCIDAD_FULL);
 analogWrite (PWM_DER, VELOCIDAD_FULL);
}

void para(){
 analogWrite (PWM_IZQ, 0);
 analogWrite (PWM_DER, 0); 
}

void atras() {
 digitalWrite (IZQ_GIR0, LOW);
 digitalWrite (IZQ_GIR1, HIGH);
 digitalWrite (DER_GIR0, LOW);
 digitalWrite (DER_GIR1, HIGH);
 analogWrite (PWM_IZQ, VELOCIDAD_FULL);
 analogWrite (PWM_DER, VELOCIDAD_FULL);
}

void gira_derecha() {
 analogWrite (PWM_IZQ, VELOCIDAD_FULL);
 analogWrite (PWM_DER, VELOCIDAD_GIRO);
}

void gira_izquierda() {
 analogWrite (PWM_IZQ, VELOCIDAD_GIRO);
 analogWrite (PWM_DER, VELOCIDAD_FULL);
}





