#include <Arduino.h>
#include <WirelessUART.h>
#include <SoftwareSerial.h>

SoftwareSerial radio(10, 11); // RX, TX

WirelessUART WU;

struct RECEIVE_DATA_STRUCTURE {
	unsigned int d1;
	unsigned int d2;
	float d3;
	float d4;
	float d5;
	bool d6;
};
RECEIVE_DATA_STRUCTURE RxData;

struct RECEIVE_DATA_STRUCTURE2 {
	unsigned int d1;
	unsigned int d2;
	float d3;
	float d4;
	float d5;
	bool d6;

};
RECEIVE_DATA_STRUCTURE2 RxData2;

void setup() {
	Serial.begin(9600);
	radio.begin(9600);

	WU.begin(&radio, 1);				// (radio, this divice id)

	WU.setStructs(details(RxData), 0);  // (struct, struct id)
	WU.setStructs(details(RxData2), 8); // (struct, struct id)
	WU.receiveFromAll = false;			// true by default. if true - accept packets from all devices, if false accept only devices with id from receiveFrom array
	WU.receiveFrom[0] = 4;				// array of accepted senders id's
	WU.receiveFrom[1] = 5;				// array of accepted senders id's

}

void loop() {

	if (WU.receiveData()) {

		Serial.print("Sender ID: ");
		Serial.println(WU.sender_id);

		Serial.print("Struct ID: ");
		Serial.println(WU.struct_id);

		if (WU.struct_id == 0) {

			Serial.print("ID: ");
			Serial.println(RxData.d1);

			Serial.print("Sauna Temp: ");
			Serial.println(RxData.d3);

			Serial.print("Room Temp: ");
			Serial.println(RxData.d4);

			Serial.print("Water Temp: ");
			Serial.println(RxData.d5);

			Serial.print("Boiler: ");
			if (RxData.d6) {
				Serial.println("ON");
			}
			else {
				Serial.println("OFF");
			}
			Serial.println("");
		}
		if (WU.struct_id == 8) {

			Serial.print("ID: ");
			Serial.println(RxData2.d1);

			Serial.print("Sauna Temp: ");
			Serial.println(RxData2.d3);

			Serial.print("Room Temp: ");
			Serial.println(RxData2.d4);

			Serial.print("Water Temp: ");
			Serial.println(RxData2.d5);

			Serial.print("Boiler: ");
			if (RxData2.d6) {
				Serial.println("ON");
			}
			else {
				Serial.println("OFF");
			}
			Serial.println("");
		}
	}
}

