/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Sunil Kumar Yadav
 * @brief          : Example program to understand how to configure priority of
 * interrupt and understand how preemption work wrt IRQs.
 ******************************************************************************
 * Note: Refer preempt priority grouping.
 * NVIC_IPR0 .. NVIC_IPR59 registers, each holds 4 (8 bit) Interrupt priority
 * mapping. In STM32f411 MCU, bit [7:4] are implemented and bit [3:0] are ignored.
 * i.e. total 16 configurable priority level possible for IRQ#. If two IRQ# with same
 * priority are activated at same time the ISR with lowest IRQ# will execute first.
 ******************************************************************************
 */
// need to fix bug
#include <stdint.h>
#include <stdio.h>

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif


#define USART2_IRQ_NUM   38
#define SPI2_IRQ_NUM     36
#define TIM2_IRQ_NUM     28
#define FLASH_IRQ_NUM    4

volatile uint32_t *pBaseISPR = (volatile uint32_t *)0XE000E200;       // NVIC Interrupt Set Pending Register
volatile uint32_t *pBaseISER = (volatile uint32_t *) 0xE000E100;      // NVIC Interrupt Set enable Register
volatile uint32_t *pBaseIPR = (volatile uint32_t *) 0xE000E400;       // NVIC Interrupt priority Register

//---------------------------------------------------------------------------------------------------------------------------
/* Function to configure the priority of input IRQ no.
 * Refer Interrupt priority Register x (NVIC_IPRx) for more details.
 */
void setIRQPriority(uint8_t irqNo, uint8_t priorityVal){


	uint8_t offsetToIPRx = irqNo / 4;          // As each NVIC_IPR[0:59] register contain 4 IPx based on IRQ no.
	volatile uint32_t *ipr = pBaseIPR + offsetToIPRx;   // NVIC_IPRx register which contain IPx in one of the 4 byte

	uint8_t offsetInIPRx = (irqNo % 4) * 8;    // (irqNo % 4) => byte no. in IPRx and x 8 => bit position (nth bit : nth+8 bit)

	*ipr &= ~(0xFF) << offsetInIPRx;           // Clear existing(if any) values
	*ipr |= (priorityVal << offsetInIPRx);     // set new priority value
}

/* Function to set Interrupt Set Pending Register for input IRQ#
 */
void setIRQPendReg(uint32_t irqNo) {

	volatile uint32_t* irqReg = pBaseISER;
	uint32_t ptrOffset = irqNo/32;    // Calculate offset aka pointer address wrt IRQ no.

	irqReg += ptrOffset;
	*irqReg |= (1<< (irqNo % 32));    // set bit position as per IRQ no. to 1 enable pend state
}

/* Function to set Interrupt Enable Pending Register for input IRQ#
 */
void setIRQEnableReg(uint32_t irqNo) {

	volatile uint32_t* irqReg = pBaseISPR;
	uint32_t ptrOffset = irqNo/32;    // Calculate offset aka pointer address wrt IRQ no.

	irqReg += ptrOffset;
	*irqReg |= (1<< (irqNo % 32));    // set bit position as per IRQ no. to 1 enable pend state
}
//---------------------------------------------------------------------------------------------------------------------------

int main(void)
{
	printf("Program to understand how to configure priority of IRQ");

	// Configure the priorities of IRQs
	setIRQPriority(USART2_IRQ_NUM, 0x70);    // priority bits 7:4 only implemented
	setIRQPriority(SPI2_IRQ_NUM, 0x60);      // priority bits 7:4 only implemented: High priority wrt 0x70


	// Setting interrupt pend bit for IRQ#
	setIRQPendReg(USART2_IRQ_NUM);
	setIRQPendReg(SPI2_IRQ_NUM);

	// Enabling interrupt for IRQ#
	setIRQEnableReg(USART2_IRQ_NUM);          // Here we are triggering SPI2_IRQ by manually enabling respective enable flag
											  // to simulate what happens if high priority interrupt come while serving one IRQ

	for(;;);
}

//---------------------------------------------------------------------------------------------------------------------------
void USART2_IRQHandler(void){
	// usually IRQ will copy data from buffer to STACK for further processing
	printf("Inside USART 2 ISR\n");
	setIRQEnableReg(SPI2_IRQ_NUM);    // triggering SPI2_IRQ with higher priority and it will result in calling SPI2_IRQHandler ISR
	 while(1);                        // control reaches back after serving high priority IRQ
}

void SPI2_IRQHandler(void) {

	printf("Inside SPI 2 ISR\n");
}

void EXTI4_IRQHandler(void) {
	printf("Inside EXTI4 ISR\n");
	while(1);
}

void FLASH_IRQHandler(void){
	printf("Inside FLASH ISR\n");
}

void TIM2_IRQHandler(void ){
	// usually IRQ will copy data from buffer to STACK for further processing
	printf("Inside ITM 2 ISR\n");
	 while(1);
}
