#include "WirelessUART.h"
WirelessUART::WirelessUART()
{
	receiveFromAll = true;
	id = 0;
	sender_id = 0;
	struct_id = 0;

}
void WirelessUART::begin(Stream *theStream, uint8_t device_id) {
	_stream = theStream;
	id = device_id;
}
void WirelessUART::setStructs(uint8_t* struc_adress, uint8_t stru_size,
	
	uint8_t s_id = 0) {
	if (s_id < 11) {
		str_addr[s_id] = struc_adress;
		str_sizes[s_id] = stru_size;
	}
}

void WirelessUART::setVars(uint8_t struct_id) {

	address = str_addr[struct_id];
	size = str_sizes[struct_id];
	rx_buffer = (uint8_t*)malloc(size + 1);
}

void WirelessUART::sendData(uint8_t struct_id, uint8_t send_to_id = 0) {

	if (!((struct_id < MAX_STRUCTS_COUNT) && (str_addr[struct_id] != NULL))) {
		return;
	}

	WirelessUART::setVars(struct_id);
	uint8_t CS = size;

	_stream->write(0x06);
	_stream->write(0x85);
	_stream->write(send_to_id);
	_stream->write(id);
	_stream->write(struct_id);
	_stream->write(size);
	for (int i = 0; i < size; i++) {
		CS ^= *(address + i);
		_stream->write(*(address + i));
	}
	_stream->write(CS);
}
boolean WirelessUART::isValidSender(uint8_t sender_id) {
	for (int i = 0; i < MAX_NODES_COUNT - 1; i++) {
		if (sender_id == receiveFrom[i]) {
			return true;
		}
	}
	return false;
}

boolean WirelessUART::receiveData() {

	if (rx_len == 0) {
		if (_stream->available() >= 6) {
			while (_stream->read() != 0x06) {

				if (_stream->available() < 6)
					return false;
			}
			if (_stream->read() == 0x85) {

				uint8_t _send_to_id = _stream->read(); // read send_to_id


				if (_send_to_id != 0)
				{
					if (_send_to_id != this->id)
					{
						return false;
					}
				}

				sender_id = _stream->read(); // read sender id

				if (!receiveFromAll) {
					if (!(WirelessUART::isValidSender(sender_id))) {
						return false;
					}
				}

				struct_id = _stream->read(); // read struct_id id
				if (str_addr[struct_id] != NULL) {
					WirelessUART::setVars(struct_id);
				}
				else {
					return false;
				}
				rx_len = _stream->read();
				//make sure the binary structs on both Arduinos are the same size.

				if (rx_len != size) {
					rx_len = 0;
					return false;
				}
			}
		}
	}

	if (rx_len != 0) {
		while (_stream->available() && rx_array_inx <= rx_len) {
			rx_buffer[rx_array_inx++] = _stream->read();
		}
		if (rx_len == (rx_array_inx - 1)) {
			calc_CS = rx_len;
			for (int i = 0; i < rx_len; i++) {
				calc_CS ^= rx_buffer[i];
			}
			if (calc_CS == rx_buffer[rx_array_inx - 1]) { //CS good
				memcpy(address, rx_buffer, size);
				rx_len = 0;
				rx_array_inx = 0;
				return true;
			}

			else {
				rx_len = 0;
				rx_array_inx = 0;
				return false;
			}
		}
	}
	return false;
}



