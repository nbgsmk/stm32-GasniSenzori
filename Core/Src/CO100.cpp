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
UART_HandleTypeDef hUrt;
UART_HandleTypeDef hUrtDbg;


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

}


// stm32 specific
void CO_100::setSensorUart(UART_HandleTypeDef sensorUart) {
	hUrt = sensorUart;

}
void CO_100::setDebugUart(UART_HandleTypeDef debugUart) {
	hUrtDbg = debugUart;

}


/**
 * Perform minimal initialization
 */
void CO_100::init() {
	// stm32 specific
	HAL_Delay(10000);

//	send(cmdSetPassiveMode);
//	getProperties_D7();
//	send(cmdRunningLightOff);

//	send(cmdSetActiveMode);
//	send(cmdRunningLightOn);
}


/**
 * Query parameters from the sensor and populate struct
 */
void CO_100::getProperties_D7() {
	//
	// VAZNO!! Saljem COMMAND 4 = "D7". Odgovor je drugaciji nego za D1
	//
	send(cmdGetTypeRangeUnitDecimals0xd1);

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
//FIXME		printf("Wrong sensor type! Expected 0x19, got |%04x|", tip);
//FIXME		throw std::invalid_argument("Wrong sensor type! Expected 0x19, got ??");		// FIXME dodati ovde hex parametar tip, izbaciti printf
	}


}


/**
 * Set active mode: sensor will send measurement data automatically in 1 second intervals
 */
void CO_100::setActiveMode() {
	send(cmdSetActiveMode);
}


/**
 * Set passive mode: sensor will send measurement data only when requested
 */
void CO_100::setPassiveMode() {
	send(cmdSetPassiveMode);
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
	send(cmdReadGasConcentration);

	// TODO odgovor iz interrupta
	vector<uint8_t> reply = receive9();
	bool hdr = (reply[0]==0xFF)  && (reply[1]==0x86);	// reply header ok?
	if (hdr) {
		uint16_t max = (reply[4] << 8) | (reply[5]);
		uint16_t ppm = (reply[6] << 8) | (reply[7]);
		rezultat = ppm;
	} else {
//		rezultat = SENSOR_DATA_ERROR;	// TODO ukljuciti na kraju
	}
	return rezultat;
}


/**
 * @return gas concentration normalized to 0~100% of max measurement range
 */
uint16_t CO_100::getGasPercentageOfMax() {
	uint16_t rezultat;
	send(cmdReadGasConcentration);

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
	send(cmdRunningLightOn);
}


/**
 * Sensor activity led will be off
 */
void CO_100::setLedOff() {
	send(cmdRunningLightOff);
}


/**
 * Sensor activity led will be off
 */
void CO_100::getLedStatus() {
	send(cmdRunningLightGetStatus);
}



//////////////////
// pomocnici
//////////////////

/**
 * dummy function to send commands to sensor
 */
void CO_100::send(const CmdStruct_t tx) {
	int s = tx.cmd.size();
	uint8_t cmdArray[s];
	std::copy(tx.cmd.begin(), tx.cmd.end(), cmdArray);

	// stm32 specific
	// send bytes
//    uint8_t c[] = "\n cmd=";
//    HAL_UART_Transmit(&hUrtDbg, c, sizeof(c), 500);
//    HAL_UART_Transmit(&hUrtDbg, cmdArray, sizeof(cmdArray), 500);


    // send command and immediately wait to receive reply of tx.expectedReplyLen bytes
    HAL_UART_Transmit(&hUrt, cmdArray, sizeof(cmdArray), 500);
	if (tx.expectedReplyLen > 0) {
		if(HAL_UART_Receive(&hUrt, rxB, tx.expectedReplyLen, 10000)==HAL_OK){
		    __NOP();
		    uint8_t r[] = " reply=";
		    HAL_UART_Transmit(&hUrtDbg, r, sizeof(r), 500);
		    HAL_UART_Transmit(&hUrtDbg, rxB, tx.expectedReplyLen, 500);
		    uint8_t c[] = ";\n";
		    HAL_UART_Transmit(&hUrtDbg, c, sizeof(c), 500);
		} else {
		    __NOP();
		    uint8_t jbg[] = "\n no reply \n";
		    HAL_UART_Transmit(&hUrtDbg, jbg, sizeof(jbg), 500);
		}
	}


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


void CO_100::sendCmd(const uint8_t *plainTxt, uint16_t size){
	std::vector<uint8_t> s(plainTxt, plainTxt + size);
	CmdStruct_t cmd = { s, size };
	send(cmd);
}


