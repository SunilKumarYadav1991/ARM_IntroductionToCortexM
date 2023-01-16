/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Sunil Kumar Yadav
 * @brief          : Example program to understand how stack is used in ARM
 * processor. In this example we are using 2KB stack of which 1KB will be used
 * as stack in handler mode using PSP and remaining 1K stack will use PSP in
 * thread mode. Using assembly instruction we will manipulate the SP.
 ******************************************************************************
 * Note: In thread mode SP uses MSP by default. Unless user modifies SPSEL in
 * CONTROL register it will continue to use MSP.
 *
 * In handler mode processor uses PSP as SP. In handler mode there is no effect
 * of changing SP via SPSEL bit.
 *
 * Note: ARM CORTEX-M4 has full descending stack model. One has to be careful
 * with stack consumption model and design code accordingly. Other type of stack
 * consumption are full descending, full ascending, empty descending and empty
 * ascending.
 *
 ******************************************************************************
 */

#include <stdint.h>
#include<stdio.h>

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif


// refer Arm Cortex Manual for more details.
#define SRAM_START        0x20000000U
#define SRAM_SIZE        (128*1024)     /*128KB SRAM*/
#define SRAM_END         ((SRAM_START) + (SRAM_SIZE))

#define MSP_START        SRAM_START
#define MSP_END 		 (MSP_START - 1024)
#define PSP_START		 MSP_END


/* Dummy function
 * */
int addition(int a, int b, int c, int d);

/*
 * Function to change SP to use PSP
 * As PSP is internal register in ARM we need to use naked function
 * Note: As we can not use earlier macros, we will redefine in assembly way
 * */
__attribute__((naked)) void changeSP2PSP(void);

/*
 * Function to trigger exception to enter in handler mode.
 * Monitor SP once handler mode is entered
 * */
void generateException(void);

/*
 * SVC exception handler
 * */
void SVC_Handler(void);

int main(void)
{
	printf("Program start in Thread Mode.\nChanging SP to use PSP from default MSP");
	changeSP2PSP();


	int result=0;
	result = addition(10, 20, 30, 40);

	printf("Result = %d\n",result);
	generateException();
    /* Loop forever */
	for(;;);
}


int addition(int a, int b, int c, int d) {

	return (a + b + c + d);
}


__attribute__((naked)) void changeSP2PSP(void) {

	__asm volatile(".equ SRAM_END, (0x20000000 + (128*1024)) ");
	__asm volatile(".equ PSP_START, (SRAM_END - 1024) ");
	__asm volatile("LDR R0,=PSP_START");
	__asm volatile("MSR PSP, R0");                       // assign MSP with value of PSP_START. (valid address before SP -> PSP)
	__asm volatile("MOV R0, #0x02");
	__asm volatile("MSR CONTROL, R0");                  // For SPSEL (1st bit) set to 1 result into PSP as current SP.
	__asm volatile("BX LR");                            // branch and return to address pointed by LR
}

void generateException(void) {
	__asm volatile("SVC #0x02");    // SVC instruction to cause SVC exception.
}


void SVC_Handler(void) {
	printf("Inside SVC exception handler\n");
}

