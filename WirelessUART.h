/******************************************************************
 * WirelessUART Arduino Library v 1.00 Beta
 * based on EasyTransfer Arduino Library
 *
 *	dene@bk.ru
 ******************************************************************/
#ifndef WirelessUART_h
#define WirelessUART_h

 //make it a little prettier on the front end. 
#define details(name) (byte*)&name,sizeof(name)

//Not necessary, but just in case.
#if ARDUINO > 22
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "Stream.h"

#define MAX_STRUCTS_COUNT 10	// max struct count
#define MAX_NODES_COUNT 5		// max count of senders

class WirelessUART {
public:
	WirelessUART();
	void begin(Stream *theStream, uint8_t);
	void setStructs(uint8_t *, uint8_t, uint8_t);
	void sendData(uint8_t, uint8_t);
	bool receiveData();
	Stream *_stream;
	bool receiveFromAll;
	uint8_t receiveFrom[MAX_NODES_COUNT];
	uint8_t id;								//this device id
	uint8_t sender_id;						//sender id
	uint8_t struct_id = 0;					//struct id

private:
	void setVars(uint8_t);
	bool isValidSender(uint8_t);
	uint8_t* str_addr[MAX_STRUCTS_COUNT];	// array of struct pointers
	uint8_t str_sizes[MAX_STRUCTS_COUNT];	// array of struct sizes
	uint8_t *address;  						//address of structure
	uint8_t size;       					//size of structure
	uint8_t *rx_buffer; 					//address for temporary storage and parsing buffer
	uint8_t rx_array_inx;  					//index for RX parsing buffer
	uint8_t rx_len;							//RX packet length according to the packet
	uint8_t calc_CS;	   					//calculated Checksum
	uint8_t send_to_id;						//send to id
};

#endif
