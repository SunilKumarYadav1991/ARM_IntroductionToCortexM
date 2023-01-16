/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Sunil Kumar Yadav
 * @brief          : Example program to understand how to enable, disable or
 * trigger interrupt in ARM CORTEX.
 ******************************************************************************
 * Note: Please refer NVIC section of ARM generic user guide for understanding
 * more on various registers. E.g.
 * Interrupt Set-enable Register: Sets respective bit to enable # interrupt
 * Interrupt Clear-enable Register: Clear respective bit to disable # interrupt
 * Interrupt Set-Pending Register: Set respective bit to set # interrupt in pending state
 * Interrupt Clear-Pending Register: Clears respective bit of # interrupt to not pending state
 * Note: Reading and writing 0/1 has different meaning. So please refer doc for
 * further information. For interrupt no. please refer vendor specific doc.
 *
 * Note: IRQ # sart from 0. Refer startup*.s file. e.g. WWDG_IRQHandler = IRQ0
 * For more information refer pm0214-stm32-cortexm4-mcus-and-mpus-programming-manual-stmicroelectronics.pdf
 *
 * Note: IRQ# and (preempt) priority & sub-priority no. are different.
 *
 ******************************************************************************
 */

#include <stdint.h>
#include<stdio.h>

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif


#define USART1_IRQ_NUM 37


int main(void)
{
	printf("Program to simulate how to enable UART IRQ manually");

	/* As we are not using UART port in this program to receive input data
	 we will manually pend required bit to one to simulate UART trigger
	 */
	volatile uint32_t *pISPR1 = (volatile uint32_t *)0XE000E204;    // Interrupt Set Pending Register 1
	*pISPR1 |= (1 << (USART1_IRQ_NUM % 32));

	/* As IRQ# are disabled by default, we need to enable the IRQs
	*/
	volatile uint32_t *pISER1 = (volatile uint32_t *) 0xE000E104;  // Interrupt Set enable Register 1
	*pISER1 |= (1 << (USART1_IRQ_NUM % 32));

	/* Since we will not be triggering USART from serial interface, USART1_IRQHandler() will not be executed
	 * as IRQ# is not triggered. But we can verify how things would work by looking at the  debugger's SFR
	 * (Special function register) and expanding required register from NVIC section (required bit would be set).
	 */

	for(;;);
}

void USART1_IRQHandler(void){
	// usually IRQ will copy data from buffer to STACK for further processing
	printf("Inside USART 1 ISR\n");
}


