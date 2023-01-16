/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Sunil Kumar Yadav
 * @brief          : Simple program to print text from CortexM4 MCU using SWD
 *
 ******************************************************************************
 */

#include <stdint.h>
#include<stdio.h>

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

int main(void)
{
	printf("Hello world STM32\n");
    /* Loop forever */
	for(;;);
}
