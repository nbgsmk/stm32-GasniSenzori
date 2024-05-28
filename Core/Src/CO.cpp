/*
 * CO.cpp
 *
 */

#include "CO.h"
#include <stdexcept>

#include <iostream>
#include <string>
#include <cstring>

CO::CO() {
	init();
}

CO::~CO() {
	// TODO Auto-generated destructor stub
}



void CO::init() {
	send(cmd_set_passive_mode);
	getProperties_D7();
//	send(cmd_running_light_off);

//	send(cmd_set_active_mode);
//	send(cmd_running_light_on);
}



void CO::getProperties_D7() {
	//
	// VAZNO!! Saljem COMMAND 4 = "D7". Odgovor je drugaciji nego za D1
	//
	send(cmd_get_type_range_unit_decimals_0xD7);

	// TODO odgovor iz interrupta
	std::vector<uint8_t> reply = receive9();
	bool hdr = (reply[0]==0xFF)  && (reply[1]==0xD7);	// reply header ok?
	uint8_t tip = reply[2];
	if (hdr && tip) {
		// sensor type ok
		sensorProperty.tip = tip;
		sensorProperty.maxRange = (reply[3] << 8) | reply[4];
		switch (reply[5]) {
			case 0x02:
				strcpy(sensorProperty.unit_str, "mg/m3 and ppm");
				break;

			case 0x04:
				strcpy(sensorProperty.unit_str, "ug/m3 and ppb");
				break;

			case 0x08:
				strcpy(sensorProperty.unit_str, "10g/m3 and %");
				break;

			default:
				strcpy(sensorProperty.unit_str, " _?_ ");
				break;
		}
		uint8_t dec = reply[6] & 0b11110000;	// decimals bit 7~4, sign bits 3~0
		uint8_t sgn = reply[6] & 0b00001111;	// decimals bit 7~4, sign bits 3~0

		dec = dec >> 4;							// spustim ih skroz na desno
		uint8_t decimals =	((dec & 0b1000)<<3) |
							((dec & 0b0100)<<2) |
							((dec & 0b0010)<<1) |
							 (dec & 0b0001);	// boKte pita sta sam dobio!

		uint16_t sign =		((sgn & 0b1000)<<3) |
							((sgn & 0b0100)<<2) |
							((sgn & 0b0010)<<1) |
							 (sgn & 0b0001);	// boKte pita sta sam dobio!
		sensorProperty.decimals = decimals;
		sensorProperty.sign = sign;


	} else {
		// ENABLE EXCEPTIONS for stm32
		// Right click the project -> Properties -> C/C++ Build -> Settings -> Tool Settings -> MCU G++ Compiler -> Miscellaneous
		// add flag -fexceptions
		printf("Wrong sensor type! Expected 0x19, got |%04x|", tip);
		throw std::invalid_argument("Wrong sensor type! Expected 0x19, got ??");		// FIXME dodati ovde hex parametar tip, izbaciti printf
	}


}


void CO::setActiveMode() {
	send(cmd_set_active_mode);
}

void CO::setPassiveMode() {
	send(cmd_set_passive_mode);
}


uint16_t CO::getMaxRange() {
	return sensorProperty.maxRange;
}

uint16_t CO::getGasConcentrationPpm() {
	uint16_t rezultat;
	send(cmd_read_gas_concentration);

	// TODO odgovor iz interrupta
	std::vector<uint8_t> reply = receive9();
	bool hdr = (reply[0]==0xFF)  && (reply[1]==0x86);	// reply header ok?
	if (hdr) {
		uint16_t max = (reply[4] << 8) | (reply[5]);
		uint16_t ppm = (reply[6] << 8) | (reply[7]);
		rezultat = ppm;
	} else {
		rezultat = SENSOR_DATA_ERROR;
	}
	return rezultat;
}


uint16_t CO::getGasPercentageOfMax() {
	uint16_t rezultat;
	send(cmd_read_gas_concentration);

	// TODO odgovor iz interrupta
	std::vector<uint8_t> reply = receive9();
	bool hdr = (reply[0]==0xFF)  && (reply[1]==0x86);	// reply header ok?
	if (hdr) {
		uint16_t max = (reply[4] << 8) | (reply[5]);
		uint16_t ppm = (reply[6] << 8) | (reply[7]);
		rezultat = ppm / max;
	} else {
		rezultat = SENSOR_DATA_ERROR;
	}
	return rezultat;	// TESTME ovako ili putem getMaxRange()?
}


void CO::setLedOn() {
	send(cmd_running_light_on);
}

void CO::setLedOff() {
	send(cmd_running_light_off);
}


//////////////////
// pomocnici
//////////////////

std::vector<uint8_t> CO::receive9() {
	std::vector<uint8_t> vector(9);
	return vector;
}



std::vector<uint8_t> CO::receive13() {
	std::vector<uint8_t> vector(13);
	return vector;
}

