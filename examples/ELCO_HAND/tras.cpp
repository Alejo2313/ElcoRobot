#include "tras.h"

uint8_t OUTPUT_BUFFER[9];
uint8_t INPUT_BUFFER[6];
uint8_t knowDevices[MAX_DEVICES];
uint8_t controlPin;
bool new_data;
uint8_t address;
uint8_t nKnowDevices;
uint8_t from;
L_STATUS status;
uint8_t dummy_data[] ={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

L_STATUS takeMeTheBus() {
	if (digitalRead(controlPin)) {
		unsigned long started = millis();
		while (digitalRead(controlPin)) {
			if (millis() - started > TIME_OUT) {
#ifdef DEBUG
				Serial.println((controlPin));
#endif
				return STATUS_TIMEOUT;
			}
		}
	}
 delay(100);
	pinMode(controlPin, OUTPUT);
	digitalWrite(controlPin, HIGH);
	return STATUS_OK;
}


L_STATUS takeTheBus() {
	digitalWrite(controlPin, LOW);
	pinMode(controlPin, INPUT);
	return STATUS_OK;
}

/*ONLY IN MASTER MODE*/
L_STATUS sendData(uint8_t from , uint8_t addr, uint8_t cmd,  uint8_t* data) {
#ifdef DEBUG
	Serial.println("Sending command....");
#endif

	Wire.beginTransmission(addr);
	uint8_t trama[] = {from, cmd, data[0],  data[1], data[2], data[3], data[4], data[5]};
	Wire.write(trama, 8);
	if (Wire.endTransmission() != 0) {
#ifdef DEBUG
		Serial.println("ERROR!.");
#endif
		takeTheBus();
		return STATUS_ERROR;
	}
#ifdef DEBUG
	Serial.println("Succesfull!.");
#endif
	return STATUS_OK;
}

/*ONLY MASTER*/

L_STATUS reqData(uint8_t addr, uint8_t* data) {
	Wire.requestFrom((int)addr, 9);
	uint8_t c = 0;
	while (Wire.available() && c < 6) {
		INPUT_BUFFER[c] = Wire.read();
		c++;
	}
	return STATUS_OK;
}


/*only in slave mode*/
L_STATUS sendData(uint8_t to,  uint8_t cmd, uint8_t* data ) {
#ifdef DEBUG
	Serial.println("Sending command....");
#endif
	putData(to, cmd, data);
	if (takeMeTheBus() != STATUS_OK) {
#ifdef DEBUG
		Serial.println("ERROR!.");
#endif
		return STATUS_ERROR;
	}
  Wire.begin(0);
#ifdef DEBUG
	Serial.println("OK");
#endif
	return takeTheBus();
}



void onreq() {
#ifdef DEBUG
	Serial.println("Sending data...");
#endif
	Wire.write(OUTPUT_BUFFER, 9);
	Wire.begin(address);
}

void onRes(int nBytes) {
/*if(nBytes < 9)
  return;*/
#ifdef DEBUG
	Serial.println("ONRES");
#endif
  delay(50);
	from = Wire.read();
	uint8_t cmd = Wire.read();
	switch (cmd) {
	case DATA_IN:
#ifdef DEBUG
		Serial.println("Receiving new data....");
#endif
		uint8_t c;
		for (c = 0; c < 6 ; c++) {
			INPUT_BUFFER[c] = Wire.read();
#ifdef DEBUG
			Serial.print((char)INPUT_BUFFER[c] );
#endif
		}
#ifdef DEBUG
		Serial.println("" );
#endif
		new_data = true;
		break;
	case AUTH:
#ifdef DEBUG
		Serial.println("Succesfull auth!." );
#endif
		Wire.flush();
		break;
	default:
		break;
	}
}

void yesSir() {
  unsigned long temp;
	 pinMode(controlPin, OUTPUT);
	 digitalWrite(controlPin, LOW);
	pinMode(controlPin, INPUT);
temp = millis();
 while(!digitalRead(controlPin)){
    if(millis() - temp > 50){
      
           pinMode(controlPin, OUTPUT);
   digitalWrite(controlPin, HIGH);
   return;
      }
  }
	if (1) {
#ifdef DEBUG
		Serial.println("YESSIR");
#endif
		Wire.requestFrom(0, 9);
		temp = millis();
		while (!Wire.available()) {
			if (millis() - temp > 100) {
				 pinMode(controlPin, OUTPUT);
				digitalWrite(controlPin, HIGH);
				return;
			}
		}


		uint8_t from = Wire.read();
		uint8_t to = Wire.read();
		uint8_t cmd = Wire.read();
		uint8_t c = 0;
#ifdef DEBUG
		Serial.println("" );
		Serial.print("From: 0x");
		Serial.println(from, HEX);
		Serial.print("to: 0x");
		Serial.println(to, HEX);
		Serial.print("command: 0x");
		Serial.println(cmd, HEX);
#endif

		switch (cmd) {
		case DATA_IN:
			while (Wire.available()) {
				INPUT_BUFFER[c] = Wire.read();
#ifdef DEBUG
				Serial.print((char)INPUT_BUFFER[c]);
#endif
				c++;
			}
			sendData(from, to, cmd, INPUT_BUFFER);
			break;

		case AUTH:
#ifdef DEBUG
			Serial.print("NEW DEVICE!");
#endif
			knowDevices[nKnowDevices] = from;
			nKnowDevices++;
			sendData(MASTER_ADDRESS, from, AUTH, dummy_data);
			Wire.flush();
			break;
		case REQ_DATA:
			if (to == MASTER_ADDRESS) {
				int c;
				for (c = 0; c < 6; c++) { //FALLOOOOOOOOOOOOOOOOOOOOO
					if (c < nKnowDevices)
						OUTPUT_BUFFER[c] = knowDevices[c];
					else
						OUTPUT_BUFFER[c] = 0;
				}
				sendData(MASTER_ADDRESS, from, DATA_IN, OUTPUT_BUFFER);
			}
			else {

			}
			Wire.flush();
			break;

		default:
			break;
		}
	}
	// pinMode(controlPin, OUTPUT);
	// digitalWrite(controlPin, HIGH);

}

void tras_begin(uint8_t addr, uint8_t control) {
	controlPin = control;
	address = addr;
	new_data = false;
#ifdef NODE
	Wire.begin(addr);
#else
	Wire.begin(addr);
#endif
	pinMode(13, OUTPUT);
	Wire.onReceive(onRes);
	Wire.onRequest(onreq);
	pinMode(controlPin, INPUT);
#ifdef DEBUG
	Serial.begin(9600);
#endif
}

void tras_begin(uint8_t control) {
	controlPin = control;
	new_data = false;
#ifdef NODE
	Wire.begin(D1, D2);
#else
	Wire.begin();
#endif
	pinMode(controlPin, OUTPUT);
	digitalWrite(controlPin, HIGH);
#ifdef DEBUG
	Serial.begin(9600);
#endif
}

L_STATUS auth() {
	sendData(0x00, AUTH, dummy_data);
	return STATUS_OK;
}

bool data_available() {
	return new_data;
}

void readData(uint8_t* data) {
	data = INPUT_BUFFER;
	new_data = false;
}

void putData(uint8_t to, uint8_t cmd, uint8_t* data) {
	int c;
	OUTPUT_BUFFER[0] = address;
	OUTPUT_BUFFER[1] = to;
	OUTPUT_BUFFER[2] = cmd;
	for (c = 0; c < 6; c++)
		OUTPUT_BUFFER[c + 3] = data[c];
}


/*rf functions */
#ifdef RF

void tras_rf_begin(uint8_t dataP, uint8_t mode) {
	vw_setup(2000);
	if (mode == RF_EMITER) {
		vw_set_tx_pin(dataP);
	}
	else {
		vw_set_rx_pin(dataP);
		vw_rx_start();
	}
}

L_STATUS rf_sendData(uint8_t addr, uint8_t cmd,  uint8_t* data) {
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

bool rf_data_available() {
	return  vw_have_message();
}

void rf_readData(uint8_t* data) {
	uint8_t len = 9;
	vw_get_message(data, &len);
}
#endif

