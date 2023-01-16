/*
 * led.c
 *
 *  Created on: 29-Nov-2022
 *      Author: Sunil Yadav
 */


#include"led.h"

// software based basic delay
void delay(uint32_t count) {
	for(uint32_t i=0; i<count; i++); // do nothing
}


/* refer memory map https://www.st.com/resource/en/datasheet/stm32f411re.pdf
 * for more details. PD12(green) PD13(orange) PD14(red) PD15(blue)
 * Enable GPIO D and set PD[12:15] as output. GPIO D is connected to AHB1 bus
 */
void initializeAllLed(void) {

	// refer Table 1. STM32F411xC/E register boundary addresses for address and corresponding section for offset
	volatile uint32_t *pRccAhb1enr = (volatile uint32_t*)0x40023830;   // RCC AHB1 peripheral clock enable register (RCC_AHB1ENR)
	volatile uint32_t *pGpiodModeReg = (volatile uint32_t*)0x40020C00;  // GPIO port mode register

	*pRccAhb1enr |= (1<<3);    // GPIODEN: enable IO port D clock

	// Set GPIO port mode register: 00 (input), 01(General purpose output mode), 10(alternate function mode), 11(analog mode)
	// below statements sets 24th, 26th, 28th and 30th bit as 1 as other bits are reset state 00
	*pGpiodModeReg |= ( 1 << (2 * LED_GREEN));   // MODER12
	*pGpiodModeReg |= ( 1 << (2 * LED_ORANGE));  // MODER13
	*pGpiodModeReg |= ( 1 << (2 * LED_RED));     // MODER14
	*pGpiodModeReg |= ( 1 << (2 * LED_BLUE));    // MODER15

	ledOff(LED_GREEN);
	ledOff(LED_ORANGE);
	ledOff(LED_RED);
	ledOff(LED_BLUE);

}

// GPIOD (0x4002 0C00) offset 0x14 for GPIOx_ODR
void ledOn(uint8_t led_no)
{
    volatile uint32_t *pGpiodDataReg = (volatile uint32_t*)0x40020C14;  // GPIO port output data register
    *pGpiodDataReg |= ( 1 << led_no);  // writing 1 to output reg i.e. power on
}

// GPIOD (0x4002 0C00) offset 0x14 for GPIOx_ODR
void ledOff(uint8_t led_no)
{
	  volatile uint32_t *pGpiodDataReg = (volatile uint32_t*)0x40020C14;  // GPIO port output data register
	  *pGpiodDataReg &= ~( 1 << led_no);   // writing 0 to output reg i.e. power off
}
