/*
 * main.h
 *
 *  Created on: 04-Dec-2022
 *      Author: Sunil Yadav
 */

#ifndef MAIN_H_
#define MAIN_H_

/* Few constants for simpler calculation */
#define TASK_STACK_SIZE           1024U
#define SCHEDULER_STACK_SIZE      1024U

#define SRAM_START                0x20000000U
#define SRAM_SIZE                 (128*1024)
#define SRAM_END				  ((SRAM_START) + (SRAM_SIZE))

#define MAX_TASKS 5  /* 4 task and 1 scheduler*/

#define T1_STACK_START            SRAM_END
#define T2_STACK_START			  ((SRAM_END) - (1*TASK_STACK_SIZE))
#define T3_STACK_START			  ((SRAM_END) - (2*TASK_STACK_SIZE))
#define T4_STACK_START			  ((SRAM_END) - (3*TASK_STACK_SIZE))
#define IDLE_STACK_START	      ((SRAM_END) - (4*TASK_STACK_SIZE))
#define SCHEDULER_STACK_START	  ((SRAM_END) - (5*TASK_STACK_SIZE))


#define TICK_HZ                    100000U        /* 1 MS time count value*/
#define HSI_CLOCK                  100000000U     /* In our STM32 board its 100MHZ after clock init*/
#define SYSTICK_TIME_CLK           HSI_CLOCK


#define DUMMY_XPSR                 0x01000000U    /* Dummy xpsr to initialize the stack*/

#define TASK_READY_STATE           0x00
#define TASK_BLOCKED_STATE         0xFF


#define INTERRUPT_DISABLE()       do{__asm volatile ("MOV R0,#0x1"); asm volatile("MSR PRIMASK,R0"); } while(0)
#define INTERRUPT_ENABLE()        do{__asm volatile ("MOV R0,#0x0"); asm volatile("MSR PRIMASK,R0"); } while(0)

#endif /* MAIN_H_ */
