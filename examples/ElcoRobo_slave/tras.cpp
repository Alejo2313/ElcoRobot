#include "tras.h"

uint8_t OUTPUT_BUFFER[6];
uint8_t INPUT_BUFFER[6];
uint8_t controlPin;
bool new_data;
uint8_t address;
L_STATUS status;

L_STATUS takeMeTheBus(){
  if(digitalRead(controlPin)){
    unsigned long started = millis();
    while(digitalRead(controlPin)){
      if(millis() - started == TIME_OUT){
        #ifdef DEBUG
          Serial.println(digitalRead(controlPin));
        #endif
        return STATUS_TIMEOUT;
      }
    }
  }
  pinMode(controlPin, OUTPUT);
  digitalWrite(controlPin, HIGH);
  delay(100);
  return STATUS_OK;
}


L_STATUS takeTheBus(){
  digitalWrite(controlPin, LOW);
  pinMode(controlPin, INPUT);
  return STATUS_OK;
}

/*ONLY IN MASTER MODE*/
L_STATUS sendData(uint8_t from ,uint8_t addr,uint8_t cmd,  uint8_t* data){
  #ifdef DEBUG
    Serial.println("Sending command....");
  #endif
  Wire.beginTransmission(addr);
  uint8_t trama[] = {from, cmd, data[0],  data[1], data[2], data[3], data[4], data[5]};
  Wire.write(trama,8);
  if(Wire.endTransmission() != 0){;
    #ifdef DEBUG
      Serial.println("ERROR!.");
    #endif
    return STATUS_ERROR;
  }
  #ifdef DEBUG
    Serial.println("Succesfull!.");
  #endif
  return STATUS_OK;
}


L_STATUS reqData(uint8_t addr, uint8_t* data){
  Wire.requestFrom(addr, 6);
  uint8_t c = 0;
  while(Wire.available() && c < 6){
    INPUT_BUFFER[c] = Wire.read();
  }
  return STATUS_OK;
}


/*only in slave mode*/
L_STATUS sendData(uint8_t* data, uint8_t cmd ){
  #ifdef DEBUG
    Serial.println("Sending command....");
  #endif
  if(takeMeTheBus() != STATUS_OK){
  #ifdef DEBUG
    Serial.println("ERROR!.");
  #endif
    return STATUS_ERROR;
  }
  uint8_t trama[] = {address, cmd, data[0],  data[1], data[2], data[3]};
  Wire.write(trama,6);
  #ifdef DEBUG
    Serial.println("OK");
  #endif
  return takeTheBus();
}


void onreq(){
  #ifdef DEBUG
    Serial.println("Sending data...");
  #endif
  Wire.write(OUTPUT_BUFFER, 6);
}

void onRes(){
  #ifdef DEBUG
        Serial.println("ONRES");
      #endif
  uint8_t from = Wire.read();
  uint8_t cmd = Wire.read();
  switch(cmd){
    case DATA_IN:
      #ifdef DEBUG
        Serial.println("Receiving new data....");
      #endif
      uint8_t c;
      for(c = 0; c < 6 ; c++){
        INPUT_BUFFER[c] = Wire.read();
        #ifdef DEBUG
          Serial.print(INPUT_BUFFER[c] );
        #endif
      }
      #ifdef DEBUG
        Serial.println("" );
      #endif
      new_data = true;
      break;
    default:
      break;
  }
}
void yesSir(){
  #ifdef DEBUG
    Serial.println("yesSir");
    Wire.requestFrom(0,6);
    int temp = millis();
    while(!Wire.available()){
        Serial.print(".");
        if(millis() - temp < TIME_OUT)
          return;
      }
    uint8_t tmp = Wire.read();
    Serial.print("From: 0x");
    Serial.println(tmp, HEX);

    tmp = Wire.read();
    Serial.print("Action: 0x");
    Serial.println(tmp, HEX);

    tmp = Wire.read();
    Serial.print("First byte: 0x");
    Serial.println(tmp, HEX);

  #endif
  
}
void tras_begin(uint8_t addr, uint8_t control){
  controlPin = control;
  address = addr;
  new_data = false;
  #ifdef NODE
    Wire.begin(addr);
  #else
     Wire.begin(addr);
  #endif
  
  Wire.onReceive(onRes);
  Wire.onRequest(onreq);
  pinMode(controlPin, INPUT);
  #ifdef DEBUG
    Serial.begin(9600);
  #endif
}
void tras_begin(uint8_t controlPin){
  controlPin = controlPin;
  new_data = false;
  #ifdef NODE
      Wire.begin(D1, D2);
  #else
    Wire.begin();
  #endif
  pinMode(controlPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(controlPin), yesSir, RISING);
  #ifdef DEBUG
    Serial.begin(9600);
  #endif
}

L_STATUS auth(){
    uint8_t data[] ={address, 0xFF,0xFF,0xFF,0xFF,0xFF};
    sendData(data, AUTH);
  }
bool data_available(){
  return new_data;
}
void readData(uint8_t* data){
  data = INPUT_BUFFER;
  new_data = false;
}

void putData(uint8_t* data){
  int c;
  for(c = 0; c < 6; c++)
    OUTPUT_BUFFER[c] = data[c];
}

#ifdef RF

/*rf functions */

void tras_rf_begin(uint8_t dataP, uint8_t mode){
    vw_setup(2000);
    if(mode == RF_EMITER){
      vw_set_tx_pin(dataP);
    }
    else{
      vw_set_rx_pin(dataP);
      vw_rx_start();
    }
}

L_STATUS rf_sendData(uint8_t addr,uint8_t cmd,  uint8_t* data){
    #ifdef DEBUG
    Serial.println("Sending command....");
  #endif
  Wire.beginTransmission(addr);
  uint8_t trama[] = {RF_ADDRESS, addr, cmd, data[0], data[1], data[2], data[3], data[4], data[5]};
  vw_send(trama, 9);
  vw_wait_tx();

  #ifdef DEBUG
    Serial.println("Succesfull!.");
  #endif
  return STATUS_OK;
}
bool rf_data_available(){
  return  vw_have_message();
}

void rf_readData(uint8_t* data){
  uint8_t len = 9;
  vw_get_message(data, &len);  
}
#endif

