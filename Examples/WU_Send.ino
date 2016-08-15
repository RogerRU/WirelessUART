#include <Arduino.h>
#include <WirelessUART.h>
#include <SoftwareSerial.h>

WirelessUART WU;
SoftwareSerial radio(11, 12);

struct SEND_DATA_STRUCTURE
{
	unsigned int d1;
	unsigned int d2;
	float d3;
	float d4;
	float d5;
	boolean d6;
};

SEND_DATA_STRUCTURE SendData1;

struct SEND_DATA_STRUCTURE2
{
	unsigned int d1;
	unsigned int d2;
	float d3;
	float d4;
	float d5;
	boolean d6;
};

SEND_DATA_STRUCTURE2 SendData2;

unsigned long previousMillis = 0;
unsigned long interval = 5000;

void SendData()
{
	SendData1.d1 = 2;
	SendData1.d2 = 1;
	SendData1.d3 = 2;
	SendData1.d4 = 3;
	SendData1.d5 = 4;
	SendData1.d6 = false;

	WU.sendData(0, 0); //Send structure with id = 0 to all (send_to_id =0)

	delay(1000);

	SendData2.d1 = 20;
	SendData2.d3 = 10;
	SendData2.d3 = 20;
	SendData2.d4 = 30;
	SendData2.d5 = 40;
	SendData2.d6 = true;

	WU.sendData(8, 6); //Send structure with id = 8 to device with id=6  (send_to_id = 6)
}

void setup()
{
	Serial.begin(9600); // Serial start
	radio.begin(9600); // SoftwareSerial start

	WU.begin(&radio, 4); // (SoftwareSerial , This device id)
	WU.setStructs(details(SendData1), 0); // Add Structure with structure id 
	WU.setStructs(details(SendData2), 8); // Add Structure with structure id
}

void loop()
{
	if (millis() - previousMillis > interval)
	{
		previousMillis = millis();
		SendData();
	}
}

