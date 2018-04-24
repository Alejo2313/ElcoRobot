// Librerias I2C para controlar el mpu6050
// la libreria MPU6050.h necesita I2Cdev.h, I2Cdev.h necesita Wire.h
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"
#include "tras.h"

// La dirección del MPU6050 puede ser 0x68 o 0x69, dependiendo 
// del estado de AD0. Si no se especifica, 0x68 estará implicito
MPU6050 sensor;

// Valores RAW (sin procesar) del acelerometro y giroscopio en los ejes x,y,z
int ax, ay, az;
int gx, gy, gz;

long tiempo_prev;
float dt;
float ang_x, ang_y, ang_z;
float ang_x_prev, ang_y_prev, ang_z_prev;
float ang_x_medido, ang_y_medido, ang_z_medido;
signed char parar = 0;
int boton = 0;

// Variables usadas por el filtro paso bajo calibrado
long f_ax,f_ay, f_az;
int p_ax, p_ay, p_az;
long f_gx,f_gy, f_gz;
int p_gx, p_gy, p_gz;
int counter=0;
boolean hacer_calibrado=true;

//Valor de los offsets
int ax_o,ay_o,az_o;
int gx_o,gy_o,gz_o;

// Variables para RF
uint8_t data_motor[6]= {0,0,0,0,0,0};
uint8_t data_motor_prev[6]= {0,0,0,0,0,0};
uint8_t data_brazo[6]= {0,0,0,0,0,0};
uint8_t data_brazo_prev[6]= {0,0,0,0,0,0};
uint8_t data_camara[6]= {0,0,0,0,0,0};
uint8_t data_camara_prev[6]= {0,0,0,0,0,0};

void setup() {
  Serial.begin(57600);    //Iniciando puerto serial
  Wire.begin();           //Iniciando I2C 
  setup_calibrado();
  setupRF();
  counter=0;              // Reseteo del contador para usarlo en el loop
  
  pinMode(2, INPUT_PULLUP); 
}

void loop() {
  // Calibrado
  loop_calibrado();
  
  if (hacer_calibrado == 0){

    boton=digitalRead(2); 
    
    // Leer las aceleraciones y velocidades angulares
    sensor.getAcceleration(&ax, &ay, &az);
    sensor.getRotation(&gx, &gy, &gz);
    
    dt = (millis()-tiempo_prev)/1000.0;
    tiempo_prev=millis();
    
    //Calcular los ángulos con acelerometro
    float accel_ang_x=atan(ay/sqrt(pow(ax,2) + pow(az,2)))*(180.0/3.14);
    float accel_ang_y=atan(-ax/sqrt(pow(ax,2) + pow(az,2)))*(180.0/3.14);
    
    //Calcular angulo de rotación con giroscopio y filtro complemento  
    ang_x = 0.98*(ang_x_prev+(gx/131)*dt) + 0.02*accel_ang_x; // 1/131=250/32768 => escalado de los valores del giróscopo
    ang_y = 0.98*(ang_y_prev+(gy/131)*dt) + 0.02*accel_ang_y;

    // El eje z hay que hacerlo con un magnetómetro para evitar errores acumulativos. Con error se puede hacer con el giróscopo
    ang_z=((gz/131)*dt + ang_z_prev); 
    
    ang_x_prev=ang_x;
    ang_y_prev=ang_y;
    ang_z_prev=ang_z;

    // Ajustar el valor del counter para movimiento hacia delante y detras
    if(counter>=100){
      ang_x_medido=ang_x;
      ang_y_medido=ang_y;
      ang_z_medido=ang_z;
      counter=0;
      //Mostrar los angulos separadas por un [tab]
//      Serial.print("Rotacion en X, Y, Z:    ");
//      Serial.print(ang_x_medido); 
//      Serial.print("   ");
//      Serial.print(ang_y_medido); 
//      Serial.print("   ");
//      Serial.print(ang_z_medido); 
//      Serial.println("   ");
  /*      
// Movimiento delante/detras
      if((ang_y_medido > -30) && (ang_y_medido < 30)){
        if(parar==0){
          // NO avanza robot
          // Serial.print("NO AVANZA   ");
          data_motor[0]=1;
        }
        else if(parar==1){
          // Serial.print("DELANTE   ");
        }
        else if(parar==-1){
          // Serial.print("ATRAS   ");
        }      
      }else if(ang_y_medido > 30){
        // Avanza robot hacia delante
        if(parar==0 || parar==-1){
          parar++;  
        }
        data_motor[0]=2;
      }
      else if(ang_y_medido < -30){
        if(parar==0 || parar==1){
          parar--;  
        }
        // Avanza robot hacia atrás
        data_motor[0]=3;
      }
    }*/

// Movimiento delante/detras
      if(ang_y_medido > 30){
        // Avanza robot hacia delante
        if(parar==0 || parar==-1){
          parar++;  
        }
      }
      else if(ang_y_medido < -30){
        if(parar==0 || parar==1){
          parar--;  
        }
      }
    }
        if(parar==0){
          // NO avanza robot
          // Serial.print("NO AVANZA   ");
          data_motor[0]=1;
        }
        else if(parar==1){
          // Serial.print("DELANTE   ");
          data_motor[0]=2;
        }
        else if(parar==-1){
          // Serial.print("ATRAS   ");
          data_motor[0]=3;
        }      
    

// Movimiento derecha/izquierda 
    if((ang_x > -30) && (ang_x < 30)){
      // NO gira robot
      //Serial.println("NO GIRA   ");
      data_motor[1]=1;
      data_motor[3]=0;
    }
    else if(ang_x > 30){
      // Gira robot a derecha
      //Serial.println("GIRA DERECHA   ");
      data_motor[1]=2;
      data_motor[3]=0;
      sendRFmotores();
    }
    else if(ang_x < -30 ){
      // Gira robot a izquierda
      //Serial.println("GIRA IZQUIERDA   ");
      data_motor[1]=3;
      data_motor[3]=0;
      sendRFmotores();
    }

// Movimiento cámara: izquierda (0->90) y derecha (90->180)
      data_camara[0]=270-((ang_z+256)*0.703); // escalado 0.703=180/256 => rango = [-180º,180º]
      data_camara[0]=(data_camara[0]/10)*10;    // Discretización de los ángulos de la cámara cada 10º
    // Serial.println(boton);

    if((boton == LOW) &&(counter==10)){
      // Para motor y Byte4=1
      data_brazo[0]=data_camara[0];
      sendRFbrazo();
      data_brazo[0]=0;
    }
    
    if((data_motor_prev[0] != data_motor[0])){
      sendRFmotores();
    }

    if((data_camara_prev[0] != data_camara[0])){
      sendRFcamara();
    }

    for(int i=0; i<6; i++){
      data_motor_prev[i]= data_motor[i];
      data_brazo_prev[i]= data_brazo[i];
      data_camara_prev[i]= data_camara[i];
    } 

    
    counter++;

    // Cambiar el delay para controlar la velocidad de giro del robot (es lo que va mas rapido)
    delay(10);
  }
}

void setup_calibrado(){
  Serial.begin(57600);   //Iniciando puerto serial
  Wire.begin();           //Iniciando I2C  
  sensor.initialize();    //Iniciando el sensor

  if (sensor.testConnection()) Serial.println("Sensor iniciado correctamente");

  // Leer los offset los offsets anteriores
  ax_o=sensor.getXAccelOffset();
  ay_o=sensor.getYAccelOffset();
  az_o=sensor.getZAccelOffset();
  gx_o=sensor.getXGyroOffset();
  gy_o=sensor.getYGyroOffset();
  gz_o=sensor.getZGyroOffset();
  
  Serial.println("Offsets:");
  Serial.print(ax_o); Serial.print("\t"); 
  Serial.print(ay_o); Serial.print("\t"); 
  Serial.print(az_o); Serial.print("\t"); 
  Serial.print(gx_o); Serial.print("\t"); 
  Serial.print(gy_o); Serial.print("\t");
  Serial.print(gz_o); Serial.print("\t");
  
  Serial.println("Calibrado-----------------------------------------------------------------------------");  
}

void loop_calibrado(){
  if(hacer_calibrado==1){
    // Leer las aceleraciones y velocidades angulares
    sensor.getAcceleration(&ax, &ay, &az);
    sensor.getRotation(&gx, &gy, &gz);
  
    // Filtrar las lecturas
    f_ax = f_ax-(f_ax>>5)+ax;
    p_ax = f_ax>>5;
  
    f_ay = f_ay-(f_ay>>5)+ay;
    p_ay = f_ay>>5;
  
    f_az = f_az-(f_az>>5)+az;
    p_az = f_az>>5;
  
    f_gx = f_gx-(f_gx>>3)+gx;
    p_gx = f_gx>>3;
  
    f_gy = f_gy-(f_gy>>3)+gy;
    p_gy = f_gy>>3;
  
    f_gz = f_gz-(f_gz>>3)+gz;
    p_gz = f_gz>>3;
  
    //Cada 100 lecturas corregir el offset
    if (counter==100){
      //Mostrar las lecturas separadas por un [tab]
      Serial.print("promedio:"); Serial.print(" ");
      Serial.print(p_ax); Serial.print("\t");
      Serial.print(p_ay); Serial.print("\t");
      Serial.print(p_az); Serial.print("\t");
      Serial.print(p_gx); Serial.print("\t");
      Serial.print(p_gy); Serial.print("\t");
      Serial.println(p_gz);
  
      //Calibrar el acelerometro a 1g en el eje z (ajustar el offset)
      if (p_ax>0) ax_o--;
      else {ax_o++;}
      if (p_ay>0) ay_o--;
      else {ay_o++;}
      if (p_az-16384>0) az_o--;
      else {az_o++;}
      
      sensor.setXAccelOffset(ax_o);
      sensor.setYAccelOffset(ay_o);
      sensor.setZAccelOffset(az_o);
  
      //Calibrar el giroscopio a 0º/s en todos los ejes (ajustar el offset)
      if (p_gx>0) gx_o--;
      else {gx_o++;}
      if (p_gy>0) gy_o--;
      else {gy_o++;}
      if (p_gz>0) gz_o--;
      else {gz_o++;}
      
      sensor.setXGyroOffset(gx_o);
      sensor.setYGyroOffset(gy_o);
      sensor.setZGyroOffset(gz_o);    
  
      counter=0;
    }  
    
    if( (p_ax < 500) && (p_ax > -100)&&
        (p_ay < 500) && (p_ay > -100)&&
        (p_az < 17000) && (p_az > 16000)&&
        (p_gx < 100) && (p_gx > -100)&&
        (p_gy < 100) && (p_gy > -100)&&
        (p_gy < 100) && (p_gy > -100)){
          Serial.println("Fin de calibrado-------------------------------------------------------------");
          hacer_calibrado = 0;
    }       
  }
  counter++;
}

void setupRF(){
  tras_rf_begin(4, RF_EMITER);
}

void sendRFmotores(){
  Serial.println("Envía dato a motores");
  
  for(int i=0; i<6; i++){
    Serial.print(data_motor[i]);
  }
  Serial.println();
  rf_sendData(0x04,DATA_IN, data_motor);
  rf_sendData(0x04,DATA_IN, data_motor);
  rf_sendData(0x04,DATA_IN, data_motor);
}
void sendRFbrazo(){
  Serial.println("Envia dato a brazo");
  rf_sendData(0x03,DATA_IN, data_brazo);
  rf_sendData(0x03,DATA_IN, data_brazo);
  rf_sendData(0x03,DATA_IN, data_brazo);
}
void sendRFcamara(){
  Serial.print("Giro cámara:   ");
  Serial.println(data_camara[0]);
  
  rf_sendData(0x02,DATA_IN, data_camara);
  rf_sendData(0x02,DATA_IN, data_camara);
  rf_sendData(0x02,DATA_IN, data_camara);
}

