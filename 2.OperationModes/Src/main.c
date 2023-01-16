/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Sunil Kumar Yadav
 * @brief          : This program explores operation modes of ARM Cortex-M processor.
 * There are two operational mode:
 * 1. Thread Mode(user mode): User program runs in this mode.
 * 2. Handler mode: Exception and fault handling code run in this mode.
 * Note: Since cortex M processors have band stacks i.e Process Stack Poiner and
 * Main Stack Pointer, in OS like environment kernal code uses MSP as SP and PSP
 * as SP in user mode.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 *
 ******************************************************************************
 */

#include <stdint.h>
#include<stdio.h>

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif


/** Function to understand how to use inline assembly in C
 */
void assembly_inline_example(void);


/** Function to modify access level
 */
void changeAccessLevelUnprivilege(void);


/**
 * Function pointer to understand importance of T bit
 */

void (*funcPtr)(void);

void foo(void) {
	// do something
}

/** This function executes in Thread Mode of the processor
 */
void generateInterrupt(void) {

	volatile uint32_t *pSTIR = (uint32_t*) 0xE000EF00;   // Software Trigger Interrupt Register
	volatile uint32_t *pISER0 = (uint32_t*) 0xE000E100;  // Interrupt Set Enable Register

	*pISER0 |= (1 << 3);                         // enable IRQ3 interrupt
	*pSTIR = (3 & 0x1FF);                        // enable IRQ3 interrupt (0:8) bit used for IRQ#
}

int main(void) {

	printf("Before Interrupt : Thread Mode\n");

	// comment out to use generate exception exercise.
	//changeAccessLevelUnprivilege();   // after entering unprivileged access level, program has limited access.
										// hence trying to generate exception using generateInterrupt() will result in hard fault

	generateInterrupt();

	printf("After Interrupt : Thread Mode\n");

	//-----------------------------------------------------------------------------------
	// comment changeAccessLevelUnprivilege to run T bit importance exercise
	funcPtr=foo;                    // Compiler will update 0th bit of address to 1 to indicate this is Thumb instruction. Address of foo = 0x080001d8
	funcPtr();						// 0th bit of function address it copied to T-bit

	funcPtr=(void*) 0x080001d8;     // Assigning address of foo() directly. This will cause Hard fault exception as Cortex M does not have Arm(32 bit) instruction support
	funcPtr();						// last bit is not 1 and ARM ISA -> exception
	//-----------------------------------------------------------------------------------

	// assembly_inline_example(); // un-comment if needed
    /* Loop forever */
	for(;;);
}

/*
* This function(ISR) executes in HANDLER MODE of the processor
*/
void RTC_WKUP_IRQHandler(void) {               // IRQ3: RTC Wakeup interrupt through the EXTI line

	// Monitor Interrupt Program Status Register (IPSR) part of xPSR Special register once ISR execute.
	// IPSR will contain ISR_NUMBER in [8:0] bits. 0 Thread mode and non zero handler mode(refer doc)
	// In debug mode monitor xPSR register: last 8 bits would be 19 which is 3rd IQR
	printf("Inside RTC Wakeup Interrupt Handler: Handler Mode\n");
}

void HardFault_Handler(void) {
	printf("Inside Hardfault Interrupt Handler: Handler Mode\n");
}



