/*
 * UartMux.cpp
 *
 *  Created on: May 23, 2024
 *      Author: peca
 */

// stm32 specific
#include "main.h"

// hardware driver
#include "UartMux.h"

UartMux::UartMux() {

}

UartMux::~UartMux() {

}


void UartMux::setAdr(MuxAdr_t muxAdresa){
	muxAdresa--;	// oduzmem 1 jer su s3-s1=[0..7] a nazivi uart porova=[1..8]
	currentAddress = muxAdresa;
	uint16_t rez = 0;
	rez = (muxAdresa & 0b001) ? (rez | UartMuX_1_Pin) : rez;
	rez = (muxAdresa & 0b010) ? (rez | UartMuX_2_Pin) : rez;
	rez = (muxAdresa & 0b100) ? (rez | UartMuX_3_Pin) : rez;

	// stm32 specific
	HAL_GPIO_WritePin(GPIOB, UartMuX_1_Pin|UartMuX_2_Pin|UartMuX_3_Pin, GPIO_PIN_RESET);	// write zero
	HAL_GPIO_WritePin(GPIOB, rez, GPIO_PIN_SET);	// set only necessary bits

}


MuxAdr_t UartMux::getAdr() {
	return currentAddress;
}

