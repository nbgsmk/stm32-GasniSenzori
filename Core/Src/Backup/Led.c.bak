/*
 * Led.c
 *
 *  Created on: May 27, 2024
 *      Author: peca
 */


#include "main.h"


void ledOn() {
	HAL_GPIO_WritePin(BOARD_LED_GPIO_Port, BOARD_LED_Pin, GPIO_PIN_RESET);
}

void ledOff() {
	HAL_GPIO_WritePin(BOARD_LED_GPIO_Port, BOARD_LED_Pin, GPIO_PIN_SET);
}

void ledTogl() {
	HAL_GPIO_TogglePin(BOARD_LED_GPIO_Port, BOARD_LED_Pin);
}

void ledBlink(uint32_t onTicks, uint32_t offTicks) {
	ledOn();
	HAL_Delay(onTicks);
	ledOff();
	HAL_Delay(offTicks);
}

void ledBlinkCnt(uint32_t count, uint32_t onTicks, uint32_t offTicks) {
	for (int i = 0; i < count; ++i) {
		ledBlink(onTicks, offTicks);
	}
}

void ledBlinkCntPer(uint32_t count, uint32_t onTicks, uint32_t offTicks, uint32_t periodTicks) {
	ledBlinkCnt(count, onTicks, offTicks);
	uint32_t blinkanje = count * (onTicks + offTicks);
	if (periodTicks > blinkanje) {
		uint32_t ostatak = periodTicks - blinkanje;
		HAL_Delay(ostatak);
	} else {
		HAL_Delay(1);
	}
}




