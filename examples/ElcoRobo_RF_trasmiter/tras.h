#ifndef tras_Code_h
#define  tras_Code_h

#include <Arduino.h>
#include <Wire.h>
#include <VirtualWire.h>

#define DEBUG
//#define NODE
/*definiciones*/
  //comando

#define DATA_IN   0x88
#define AUTH    0x55


//otros
#define TIME_OUT  1000


#define MASTER_ADDRESS	0x00
#define RF_ADDRESS		0xFF

#define RF_EMITER		1
#define RF_RECEPTOR		0


/*variables */
extern uint8_t OUTPUT_BUFFER[6];
extern uint8_t INPUT_BUFFER[6];
extern uint8_t controlPin;
extern bool new_data;
extern uint8_t address;
const uint8_t dummy_data[] ={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
extern uint8_t dataPin;
typedef enum L_STATUS{
   STATUS_OK,
   STATUS_ERROR,
   STATUS_TIMEOUT
};


extern L_STATUS status;


void tras_begin(uint8_t addr, uint8_t controlPin);
void tras_begin(uint8_t controlPin);
void putData(uint8_t* data);
void readData(uint8_t* data);
bool data_available();
L_STATUS auth();
/*only in slave mode*/
L_STATUS sendData(uint8_t* data, uint8_t cmd );
/*ONLY IN MASTER MODE*/
L_STATUS sendData(uint8_t from ,uint8_t addr,uint8_t cmd,  uint8_t* data);
L_STATUS reqData(uint8_t addr, uint8_t* data);

/*RF functions*/
void tras_rf_begin(uint8_t dataP, uint8_t mode);
L_STATUS rf_sendData(uint8_t addr,uint8_t cmd,  uint8_t* data);
bool rf_data_available();
void rf_readData(uint8_t* data);



#endif
