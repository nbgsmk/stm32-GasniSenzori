/*
 * Driver for "TB600C CO 100ppm" sensor
 * CO100.cpp
 *
 */

// stm32 specific
#include "main.h"
#include "usb_device.h"
#include "usbd_cdc.h"
#include "usbd_cdc_if.h"
UART_HandleTypeDef uart;

// C++ standard
#include <stdexcept>
#include <iostream>
#include <string>
#include <cstring>
#include <array>
using namespace std;

// hardware driver
#include "CO100.h"



/**
 * Constructor to create sensor and perform minimal initialization
 */
CO_100::CO_100() {

}

CO_100::~CO_100() {
	// TODO Auto-generated destructor stub
}


// stm32 specific
void CO_100::setUartHandle(UART_HandleTypeDef sensuart) {
	uart = sensuart;

}


/**
 * Perform minimal initialization
 */
void CO_100::init() {
	send(cmd_set_passive_mode);
	getProperties_D7();
//	send(cmd_running_light_off);

//	send(cmd_set_active_mode);
//	send(cmd_running_light_on);
}


/**
 * Query parameters from the sensor and populate struct
 */
void CO_100::getProperties_D7() {
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


/**
 * Set active mode: sensor will send measurement data automatically in 1 second intervals
 */
void CO_100::setActiveMode() {
	send(cmd_set_active_mode);
}


/**
 * Set passive mode: sensor will send measurement data only when requested
 */
void CO_100::setPassiveMode() {
	send(cmd_set_passive_mode);
}


/**
 * @return maximum measurement range from sensor properties
 */
uint16_t CO_100::getMaxRange() {
	return sensorProperty.maxRange;
}

/**
 * @return current gas concentration in ppm
 */
uint16_t CO_100::getGasConcentrationPpm() {
	uint16_t rezultat;
	send(cmd_read_gas_concentration);

	// TODO odgovor iz interrupta
	vector<uint8_t> reply = receive9();
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


/**
 * @return gas concentration normalized to 0~100% of max measurement range
 */
uint16_t CO_100::getGasPercentageOfMax() {
	uint16_t rezultat;
	send(cmd_read_gas_concentration);

	// TODO odgovor iz interrupta
	vector<uint8_t> reply = receive9();
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


/**
 * Sensor activity led will blink during operation
 */
void CO_100::setLedOn() {
	send(cmd_running_light_on);
}


/**
 * Sensor activity led will be off
 */
void CO_100::setLedOff() {
	send(cmd_running_light_off);
}


//////////////////
// pomocnici
//////////////////

/**
 * dummy function to send commands to sensor
 */
void CO_100::send(vector<uint8_t> txCmd) {
	int s = txCmd.size();
	uint8_t txA[s];
	std::copy(txCmd.begin(), txCmd.end(), txA);

	HAL_UART_Transmit(&uart, txA, sizeof(*txA), 100);
}



/**
 * @return dummy 9-element array of measurement data (see datasheet)
 */
std::vector<uint8_t> CO_100::receive9() {
	std::vector<uint8_t> vector(9);
	return vector;
}



/**
 * @return dummy 13-element array of measurement data (see datasheet)
 */
std::vector<uint8_t> CO_100::receive13() {
	std::vector<uint8_t> vector(13);
	return vector;
}



