/* 
 Ejemplo de control de motor DC usando modulo L298
 http://electronilab.co/tienda/driver-dual-para-motores-full-bridge-l298n/
 
 Creado 16/05/14
 por Andres Cruz
 ELECTRONILAB.CO
 */

 /* 
  *  Javier Otero Martinez
  *  Miguel Garcia Maya
  *  ELCO - 2018
  *  
  */

#define IZQ_GIR1 7   //Verde
#define IZQ_GIR0 6   //Amarillo
#define DER_GIR1 4   //Naranja
#define DER_GIR0 5   //Rojo
#define DATA_PIN 10
#define PWM_IZQ  8   //Gris
#define PWM_DER  3   //Blanco 

#define VELOCIDAD_FULL 230

void setup() {
 pinMode (PWM_IZQ, OUTPUT); 
 pinMode (PWM_DER, OUTPUT);
 pinMode (IZQ_GIR0, OUTPUT);
 pinMode (IZQ_GIR1, OUTPUT);
 pinMode (DER_GIR0, OUTPUT);
 pinMode (DER_GIR1, OUTPUT);
 pinMode (DATA_PIN, INPUT);
}

void loop() {
  avanza();
  delay(2000);
  para();
  delay(2000);
  atras();
  delay(2000);
  para();
  while (1) {delay(1000);}
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





