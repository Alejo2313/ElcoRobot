#ifndef tras_Code_h
#define  tras_Code_h

#include <Arduino.h>
#include <Wire.h>

#ifdef RF
  #include <VirtualWire.h>
#endif
/*modes*/

#define DEBUG
//#define NODE


/*definiciones*/
//CMD
#define DATA_IN  	 		0x88
#define AUTH   				0x55
#define ACK           0x25
#define REQ_DATA      0x10


//otros
#define TIME_OUT 			    1000
#define MASTER_ADDRESS		0x00
#define RF_ADDRESS			  0xFF
#define RF_EMITER			    1
#define RF_RECEPTOR			  0

#define MAX_DEVICES       127


/*types*/enum L_STATUS{
   STATUS_OK,
   STATUS_ERROR,
   STATUS_TIMEOUT
};

/*variables */
extern uint8_t OUTPUT_BUFFER[9];
extern uint8_t INPUT_BUFFER[6];
extern uint8_t controlPin;
extern bool new_data;
extern uint8_t address;
extern uint8_t dataPin;
extern L_STATUS status;

extern uint8_t from;

extern uint8_t knowDevices[MAX_DEVICES];
extern uint8_t nKnowDevices;


/*public functions*/
/*		 	ONLY IN SLAVE MODE
	Inicializa el trasmisor en modo SLAVE
	@param -> addr, Direccion asignada al dispositivo
	@param -> controlPin, Pin de control.
*/
void tras_begin(uint8_t addr, uint8_t controlPin);

/*        ONLY IN MASTER MODE
	Inicializa el trasmisor en modo MASTER

	@param -> addr, Direccion asignada al dispositivo
*/
void tras_begin(uint8_t control);

/*
	Pone  los datos en el buffer de salida. Cuando el master los 
	solicita, envia los datos.

	@param -> data, array que contiene los datos
*/
void putData(uint8_t to, uint8_t cmd, uint8_t* data);

/*
	Lee los datos disponibles en el buffer de entrada

	@param -> data, Array que contienendra los datos.
*/
void readData(uint8_t* data);

/*
	Devuelve si hay datos disponible en el  buffer de entrada

	@return -> True si hay datos
*/
bool data_available();

/*
	EN DESARROLLO, NO FUNCIONA BIEN, NO USAR
*/
L_STATUS auth();

/*		ONLY IN SLAVE MODE
	Envia los datos al master

	@Param -> data,  Array de datos a enviar. DEBE TENER AL MENOS 6 ELEMENTOS
	@Param -> cmd,  Accion a realizar. @see CMD definition

	@return -> L_STATUS,  Resultado del envio
*/

L_STATUS sendData(uint8_t to,  uint8_t cmd, uint8_t* data );

/*		ONLY IN MASTER MODE
	Envia los datos al SLAVE

	@Param -> from, Origen de  los datos
	@Param ->  addr, Direccion del Slave 
	@Param -> data,  Array de datos a enviar. DEBE TENER AL MENOS 6 ELEMENTOS
	@Param -> cmd,  Accion a realizar. @see CMD definition

	@return -> L_STATUS,  Resultado del envio
*/
L_STATUS sendData(uint8_t from ,uint8_t addr,uint8_t cmd,  uint8_t* data);

/*   ONLY IN MASTER MODE
	Solicita datos al slave

	@Param -> addr, Direccion del esclavo
	@Param -> data, Array que almacenará los datos solicitados. AL MENOS 6 ELEMENTOS
*/
L_STATUS reqData(uint8_t addr, uint8_t* data);

/************* RF functions ********************/

/*        ONLY IN MASTER MODE
	Inicializa el trasmisor en modo MASTER

	@param -> dataP, Pin de datos del trasmisor 
	@param -> mode, Modo de funcionamiento.
*/

void tras_rf_begin(uint8_t dataP, uint8_t mode);

/*		
	Envia los datos al SLAVE

	@Param ->  addr, Direccion del Slave 
	@Param -> data,  Array de datos a enviar. DEBE TENER AL MENOS 6 ELEMENTOS
	@Param -> cmd,  Accion a realizar. @see CMD definition

	@return -> L_STATUS,  Resultado del envio
*/
L_STATUS rf_sendData(uint8_t addr,uint8_t cmd,  uint8_t* data);

/*
	Devuelve si hay datos disponible en el  buffer de entrada

	@return -> True si hay datos
*/
bool rf_data_available();
/*
	Lee los datos disponibles en el buffer de entrada.

	@param -> data, Array que contienendra los datos.
*/
void rf_readData(uint8_t* data);

/*
 * devuelve el numero de dispositivos conectados
 */
uint8_t nDevices(void);

/*Devuelve la lista de dispositivos conectados;
 * 
 */
uint8_t* devices(void);

void yesSir();

#endif
