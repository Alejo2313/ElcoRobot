#include <VirtualWire.h>
#include "tras.h"

#define MAX_DEVICES   127

#define RF_STATE_PIN  8 
#define STATE_LED     9

char data[]="PUTATU";



void setup()
{
  //Serial.begin(9600);
  tras_begin(3);
  tras_rf_begin(4, RF_RECEPTOR);

// tras_begin(0x02, 3);
  //auth();
}
void loop()
{
    yesSir();
    uint8_t buf[9];
    uint8_t tmp[6];
    if (rf_data_available()) 
    {
      rf_readData(buf);
      for(int i = 3; i <9; i++){
        tmp[i-3]=buf[i];
      }
      Serial.println((int)buf[1]);
      sendData(buf[0],buf[1],buf[2], tmp);
    }
}
