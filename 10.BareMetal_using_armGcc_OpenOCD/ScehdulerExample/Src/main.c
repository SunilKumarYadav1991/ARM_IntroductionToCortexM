/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Sunil Kumar Yadav
 * @brief          : Example program to understand learn how to create own basic
 * scheduler. In this example we will have few user level tasks which will run in
 * thier own designated area of stack and these tasks will be scheduled as round
 * robin fashion. For start we will use SysTick exception to schedule these task
 ******************************************************************************
 * Note: PendSV will be used to improve task scheduling later on as its allows
 * IRQs to execute without impacting any exception handling.
 *
 * Note: each task will consume 1K private stack
 * Note: Current HDK cotrxM4 core support 100MHz and SysTick timer is of 24bit
 * i.e. 16M max count (16MHz). It appears Systick count value is not configures
 * to desired accuracy.
 *
 *
 * NOTE: Refer https://www.st.com/resource/en/datasheet/stm32f411re.pdf and
 * um1842-discovery-kit-with-stm32f411ve-mcu-stmicroelectronics.pdf for block diagram etc
 ******************************************************************************
 */

#include <stdint.h>
//#include <stdio.h>

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

#include "main.h"
#include "task.h"                                  // declaration of task handlers
#include "led.h"



#define CONTROLLER_TICK_HZ 16000000   /*100MHZ*/

/** Function to configure sysTick exception registers with
 * input time passed as arg. Count resolution 0.00000001s i.e. 0.01uS
 */
void configSysTickTimer(uint32_t usTime);


/** Enable the processor faults like mem manage, bus and usage fault
 */
void enableProcessorFaults(void);


/** Initializes the scheduler task using input param top/start of scheduler stack
 */
__attribute__((naked)) void initSchedulerStack(uint32_t schedStackTop);


/** Initialized different task stack (four in this case)
 */
void initTaskStack(void);


/** switch SP to PSP before starting task in thread mode
 */
__attribute__((naked)) void switchSpToPsp(void);


/** return psp value of currently running task. Used within switchSpToPSP() naked function
 */
uint32_t getPspValue(void);


/** sets input PSP value to currently running task's PSP value. Used within PendSV_Handler() naked function
 */
void setPspValue(uint32_t currentPspValue);


/** Sets the PendSV bit PENDSVSET in Interrupt control and status register to off load scheduling and
 * allows other high priority exception to be serviced. Note PendSV exception has lowest priority.
 */
void schedule(void);

/** Unblock the task based on its current state and tick/block counts
 */
void unblockTask(void);

//-------------------------------------------------------------------------------------------------------------

tcb_t g_userTasks[MAX_TASKS];            // task control block holds task related private data
uint8_t g_currentTask = 1;               // global variable to track current executing task handler. init with Task1
uint32_t g_tickCount = 0;                // tick count gets updated from sysTick handler for each systick interrupt

//-------------------------------------------------------------------------------------------------------------
int main(void)
{


    enableProcessorFaults();
    initSchedulerStack(SCHEDULER_STACK_START);

    initTaskStack();
    initializeAllLed();

    configSysTickTimer(TICK_HZ);     // 1ms

    switchSpToPsp();

    taskHandler1();

    /*
    configSysTickTimer(2);

    initializeAllLed();

    for(;;) {   // need to use systick to trigger on off
        ledOff(LED_ORANGE);
        delay(2000000);
        ledOn(LED_ORANGE);
        delay(2000000);
    }
    */
}

//-------------------------------------------------------------------------------------------------------------
void configSysTickTimer(uint32_t TimeHz) {
    volatile uint32_t* pSRLVR = (volatile uint32_t*)0xE000E014;  // STK_LOAD: SysTick reload value register
    volatile uint32_t* pSCSR = (volatile uint32_t*)0xE000E010;   // STK_CTRL: SysTick control and status register

    uint32_t tickcount = (SYSTICK_TIME_CLK/TimeHz) -1;

    *pSRLVR &= ~(0x00FFFFFF);              // Reset the counter value (24Bit) hence max time count 0.16s with 100MHz clock
    *pSRLVR |= tickcount;

    *pSCSR |= (1<<1);                       // TICKINT
    *pSCSR |= (1<<2);                       // CLKSOURCE 1: processor clock (0: processor clock/8)
    *pSCSR |= (1<<0);                       // Enable

}

//-------------------------------------------------------------------------------------------------------------

void enableProcessorFaults(void) {
    volatile uint32_t* pSHCSR = (volatile uint32_t*) 0xE000ED24;   // system handler control and state register

    *pSHCSR |= (1 << 16);                                          // mem manage fault
    *pSHCSR |= (1 << 17);                                          // bus fault
    *pSHCSR |= (1 << 18);                                          // usage fault
}


__attribute__((naked)) void initSchedulerStack(uint32_t schedStackTop) {
    __asm volatile("MSR MSP, %0" : : "r" (schedStackTop) : );
    __asm volatile("BX LR");                           // branch indirect to Link register return to calling main()
}


void initTaskStack(void) {

    // Initialize each task's state (initial state) as ready state
    for(uint8_t i=0; i < MAX_TASKS; ++i) {
        g_userTasks[i].currentState = TASK_READY_STATE;
    }

    // Initialize process stack pointer of each task
    g_userTasks[0].pspValue = IDLE_STACK_START;
    g_userTasks[1].pspValue = T1_STACK_START;
    g_userTasks[2].pspValue = T2_STACK_START;
    g_userTasks[3].pspValue = T3_STACK_START;
    g_userTasks[4].pspValue = T4_STACK_START;

    // Initialize task handler to point appropriate task function
    g_userTasks[0].taskHandlerFptr = idleTaskHandler;
    g_userTasks[1].taskHandlerFptr = taskHandler1;
    g_userTasks[2].taskHandlerFptr = taskHandler2;
    g_userTasks[3].taskHandlerFptr = taskHandler3;
    g_userTasks[4].taskHandlerFptr = taskHandler4;

    // store stack frames of each task. Used in restoring context of aborted task when running round robin scheduler
    uint32_t *pPSP;

    for(uint8_t i =0; i < MAX_TASKS; ++i) {
        pPSP = (uint32_t*) g_userTasks[i].pspValue;         // using top (full descending) of stack

        pPSP--;
        *pPSP = DUMMY_XPSR;

        pPSP--;                // PC : R15
        *pPSP = (uint32_t) g_userTasks[i].taskHandlerFptr;

        pPSP--;                // LR : R14
        *pPSP = 0xFFFFFFFD;    /* Return to Thread mode, exception return uses non-floating-point state
                                * from the PSP and execution uses PSP after return.*/

        // populate R13 - R0 with dummy value for the first time
        for(uint8_t j=0; j < 13; ++j) {
            pPSP--;
            *pPSP = 0;
        }

        g_userTasks[i].pspValue = (uint32_t) pPSP;
    }
}


uint32_t getPspValue(void) {
    return g_userTasks[g_currentTask].pspValue;
}


__attribute__((naked)) void switchSpToPsp(void) {
    // 1. initialize the PSP with task 1 stack start address
    // get the value of psp of current_task
    __asm volatile ("PUSH {LR}");           // preserve LR which connects back to main()
    __asm volatile ("BL getPspValue");      // branch to getPspValue(). Return stored in R0
    __asm volatile ("MSR PSP,R0");          // initialize psp with return of getPspValue()
    __asm volatile ("POP {LR}");            // pops back LR value

    // 2. change SP to PSP using CONTROL register
    __asm volatile ("MOV R0,#0X02");
    __asm volatile ("MSR CONTROL,R0");
    __asm volatile ("BX LR");
}


void setPspValue(uint32_t currentPspValue) {
    g_userTasks[g_currentTask].pspValue = currentPspValue ;
}


void updateNextTask(void) {
    uint32_t state = TASK_BLOCKED_STATE;
    // continue from here

    for(int i = 0; i < (MAX_TASKS); ++i) {
        g_currentTask++;
        g_currentTask %= MAX_TASKS;        // roll back within MAX_TASKS no. of tasks
        state = g_userTasks[g_currentTask].currentState;

        if((state == TASK_READY_STATE) && (g_currentTask != 0) )
            break;
    }

    if(state != TASK_READY_STATE)
        g_currentTask = 0;       // if no task is ready, execute idle task
}


void schedule(void)
{
    uint32_t *pICSR = (uint32_t*)0xE000ED04;      // Interrupt Control and Status Register
    *pICSR |= ( 1 << 28);                         // Set PENDSVSET bit to set the PendSV exception
}


void taskDelay(uint32_t tickCount){

    INTERRUPT_DISABLE();

    if(g_currentTask) {
        g_userTasks[g_currentTask].blockCount = g_tickCount + tickCount;
        g_userTasks[g_currentTask].currentState = TASK_BLOCKED_STATE;
        schedule();
    }

    INTERRUPT_ENABLE();
}

void unblockTask(void) {
// unblock the task which are not in ready state but thier sleep/block count has been reached
    for(uint8_t i = 0; i < MAX_TASKS; ++i) {
        if(g_userTasks[i].currentState != TASK_READY_STATE && g_userTasks[i].blockCount == g_tickCount){
            g_userTasks[i].currentState = TASK_READY_STATE;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------
__attribute__((naked)) void PendSV_Handler(void) {
    // PendSV exception handler does the task switching.

    // Save the context of current task.
    //Note : R0-R3 and R12, LR, XPSR are automatically stacked by processor

    // Get current task's PSP value to store R4-11 ad LR register.
    __asm volatile("MRS R0, PSP");                 // get the current running task's PSP value
    __asm volatile("STMDB R0!, {R4-R11}");          // Store multiple register value(PUSH), "DB" = Decrement address before each access and
                                                   // ! =  final address that is loaded from is written back to Rn
    __asm volatile("Push {LR}");
    __asm volatile("BL setPspValue");

    //-----------------------------------------------------------------------------------
    // Retrieve the context of next task.

    __asm volatile("BL updateNextTask");           // decide next task to run
    __asm volatile("BL getPspValue");              // Get latest PSP value. Note return is passed via R0 which next instruction uses
    __asm volatile("LDMIA R0!, {R4-R11}");         // Load multiple (POP), IA= Increment after each access.
    __asm volatile("MSR PSP, R0");                 // Update the value of PSP and exit
    __asm volatile("POP {LR}");
    __asm volatile("BX LR");

}


void SysTick_Handler(void) {

    //printf("Inside SysTick Handler\n");
    volatile uint32_t *pICSR = (volatile uint32_t*) 0xE000ED04;   // interrupt control and status register

    g_tickCount++;                                                // increment global tick count for each systick interrupt
    unblockTask();
    *pICSR |= ( 1 << 28);                                         // Pend the pendSV exception to deal with task switching
}


void HardFault_Handler(void){

    //printf("Inside HardFault Handler\n");
    while(1);
}

void MemManage_Handler(void){

    //printf("Inside MemManage Handler\n");
    while(1);
}

void BusFault_Handler(void){

    //printf("Inside BusFault Handler\n");
    while(1);
}
