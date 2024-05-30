/*
 * CO100.h
 *
 */

#ifndef SRC_CO100_H_
#define SRC_CO100_H_


#include <vector>
#include <stdint.h>

#include <Sensor_TB600C_CO_100ppm_datasheet.h>

#define MUX_ADR 0b001			// CONFIG mux adresa ovog senzora

class CO_100 {

public:
	CO_100();
	virtual ~CO_100();

	uint8_t rxB[50];			// bytes received from UART

	void init();				// inicijalizuj senzor, podesi passive mode, proveri tip
	void setActiveMode();
	void setPassiveMode();
	void setLedOn();
	void setLedOff();

	uint16_t getMaxRange();				// maksimalni raspon merenja senzora
	uint16_t getGasConcentrationPpm();	// koncentracija gasa ppm
	uint16_t getGasPercentageOfMax();	// koncentracija 0~100% od maksimalnog merenja senzora

	// stm32 specific and debug only
	void setSensorUart(UART_HandleTypeDef huart);
	void setDebugUart(UART_HandleTypeDef huart);
	void sendAscii(uint8_t plainTxt[]);


private:
	struct {
		uint8_t tip;
		uint16_t maxRange;
		char unit_str[15];
		uint8_t decimals;
		uint8_t sign;
	} sensorProperty;
	bool runningLed;
	void getProperties_D7();				// popuni struct sa podacima o senzoru


	// dummy funkcije umesto uarta za pocetak
	void send(const CmdStruct_t txCmd);			// dummy - posalji komande senzoru
	std::vector<uint8_t> receive9();	// dummy - vraca odgovor senzora
	std::vector<uint8_t> receive13();	// dummy - vraca odgovor senzora

};

#endif /* SRC_CO100_H_ */
