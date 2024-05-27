/*
 * Led.h
 *
 *  Created on: May 27, 2024
 *      Author: peca
 */

#ifndef SRC_LED_H_
#define SRC_LED_H_


	void ledOn();
	void ledOff();
	void ledTogl();

	void ledBlink(uint32_t onTicks, uint32_t offTicks);
	void ledBlinkCnt(uint32_t count, uint32_t onTicks, uint32_t offTicks);
	void ledBlinkCntPer(uint32_t count, uint32_t onTicks, uint32_t offTicks, uint32_t periodTicks);



#endif /* SRC_LED_H_ */
