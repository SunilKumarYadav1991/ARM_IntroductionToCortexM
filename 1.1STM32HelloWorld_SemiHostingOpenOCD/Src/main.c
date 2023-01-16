/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Sunil Yadav
 * @brief          : Understanding how to configure OpenOCD with Semi-hosting
 *  to enable debugging using printf
 ******************************************************************************
 *
 ******************************************************************************
 */

#include <stdint.h>
#include <stdio.h>

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

extern void initialise_monitor_handles(void);

int main(void) {

	initialise_monitor_handles();
	printf("Hello world! Sem-hosting\n");             // ensure all string terminate with \n
    /* Loop forever */
	for(;;);
}
