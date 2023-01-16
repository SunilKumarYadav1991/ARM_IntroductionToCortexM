/*
 * task.c
 *
 *  Created on: 22-Nov-2022
 *      Author: Sunil Yadav
 */

#include <stdio.h>
#include "task.h"
#include "led.h"

void taskHandler1(void) {
	while(1) {
		//printf("Inside task 1\n");
		ledOn(LED_GREEN);
		delay(DELAY_COUNT_1S);
		ledOff(LED_GREEN);
		delay(DELAY_COUNT_1S);
	}
}

void taskHandler2(void) {
	while(1) {
		//printf("Inside task 2\n");
		ledOn(LED_ORANGE);
		delay(DELAY_COUNT_500MS);
		ledOff(LED_ORANGE);
		delay(DELAY_COUNT_500MS);
	}
}

void taskHandler3(void) {
	while(1) {
		//printf("Inside task 3\n");
		ledOn(LED_BLUE);
		delay(DELAY_COUNT_250MS);
		ledOff(LED_BLUE);
		delay(DELAY_COUNT_250MS);
	}
}

void taskHandler4(void) {
	while(1) {
		//printf("Inside task 4\n");
		ledOn(LED_RED);
		delay(DELAY_COUNT_125MS);
		ledOff(LED_RED);
		delay(DELAY_COUNT_125MS);
	}
}


/* used for idle task if task is wasting cpu cycles and not doing productive work*/
void idleTaskHandler(void) {
	printf("Inside idle task handler\n");
	while(1) ;
}

