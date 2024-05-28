/*
 * Mux.h
 *
 *  Created on: May 23, 2024
 *      Author: peca
 */

#ifndef SRC_MUX_H_
#define SRC_MUX_H_

	#define MUX_GPIO_BITS 0b0111
	#define MUX_O2_adr	0b001
	#define MUX_H2S_adr	0b110
	#define MUX_CO2_adr	0b101
	typedef uint8_t MuxAdr_t;


class Mux {
public:
	Mux();
	virtual ~Mux();


	void setAdr(MuxAdr_t);	// prozovi uart mux adresu
	MuxAdr_t getAdr();		// koja je trenutna adresa?

	void blink(int mS);


};

#endif /* SRC_MUX_H_ */
