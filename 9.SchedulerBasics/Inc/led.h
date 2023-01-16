/*
 * led.h
 *
 *  Created on: 29-Nov-2022
 *      Author: Sunil Yadav
 *
 */

#ifndef LED_H_
#define LED_H_

#include <stdint.h>

// LED no. as per STM32F411 Discovery board
#define LED_GREEN     12
#define LED_ORANGE    13
#define LED_RED       14
#define LED_BLUE      15

#define DELAY_COUNT_1MS 		1250U
#define DELAY_COUNT_1S  		(1000U * DELAY_COUNT_1MS)
#define DELAY_COUNT_500MS  		(500U  * DELAY_COUNT_1MS)
#define DELAY_COUNT_250MS 		(250U  * DELAY_COUNT_1MS)
#define DELAY_COUNT_125MS 		(125U  * DELAY_COUNT_1MS)

void initializeAllLed(void);
void ledOn(uint8_t ledNum);
void ledOff(uint8_t ledNum);
void delay(uint32_t count);

#endif /* LED_H_ */
