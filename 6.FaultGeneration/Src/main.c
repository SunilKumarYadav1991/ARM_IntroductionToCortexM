/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Sunil Kumar Yadav
 * @brief          : Example program to understand how to use system control block.
 * In this example we will configure and enable few exception and then try to
 * trigger them to understand how fault/exceptions are handled and escalated.
 *
 * If exception handler is not implemented e.g. divide by zero (usage fault) and
 * exception is triggered then it is escalated to hard fault exception handler
 ******************************************************************************
 * Note: Refer System Handler Control and State Register for further information.
 *
 ******************************************************************************
 */

#include <stdint.h>
#include <stdio.h>

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

/* To avoid epilog/prolog while jumping to UsageFault exception handler we are
 * using this function to print stack trace and actual UsageFault exception handler
 * to simply call this function to print required stack frame data
 * */
void usageFaultHandler_c(uint32_t *pMSP);

int divide(int x, int y);

int main(void)
{
	// Enable configurable exceptions: usage fault, mem manage faule and bus fault.
	volatile uint32_t* pSHCSR = (volatile uint32_t*) 0xE000ED24;   // system handler control and state register

	*pSHCSR |= (1 << 16);                                          // mem manage fault
	*pSHCSR |= (1 << 17);                                          // bus fault
	*pSHCSR |= (1 << 18);                                          // usage fault

	//-------------------------------------------------------------------------------------------------------------
	// Enable divide by zero trap
	volatile uint32_t* pCCR = (volatile uint32_t*)0xE000ED14;      // Configure and Control Register
	*pCCR |= (1 << 4);                                             // Enables faulting or halting when the processor executes
	                                                               // an SDIV or UDIV instruction with a divisor of 0:
	// un-comment to generate and trap divide by zero exception
	// call divide by zero function to create exception
	// divide(10,0);                                                  // Results in usage fault

	//-------------------------------------------------------------------------------------------------------------
	// Simulating usage_fault using undefined instruction

	uint32_t *pSRAM = (uint32_t*) 0x20010000;        // location in SRAM to store instruction
	*pSRAM = 0xFFFFFFFF;							 // invalid instruction at SRAM location

	void (*funcPtr)(void);							 // Function pointer
	funcPtr = (void*)0x20010001;                            // initializing function pointer with pSRAM location (lsb=1 => T bit)

	funcPtr();        // As  *pSRAM  contain invalid instruction, it will result in usage fault exception
	//--------------------------------------------------------------------------------------------------------------
	/* To debug and analyze exceptions one should refer Fault status registers and fault address registers.
	 * Based on exception type there are different status registers which can be probed further
	 * For example, UsageFault Status Register, HardFault Status Register, MemManage Fault Address Register and
	 * BusFault Address Register.
	 * For usage fault analysis one can print value of 0xE000ED2A(UsageFault Status Register) and come to conclusion
	 * which is causing the issue. Refer ARM user guide.
	 * */
    /* Loop forever */
	for(;;);
}

int divide(int x, int y) {
	return x/y;
}


void HardFault_Handler(void) {

	printf("Inside HardFault Handler\n");
	while(1);
}

void BusFault_Handler(void) {

	printf("Inside BusFault Handler\n");
	while(1);
}

void MemManage_Handler(void) {

	printf("Inside MemManage Handler\n");
	while(1);
}
 /*
void UsageFault_Handler(void) {

	printf("Inside UsageFault Handler\n");
	while(1);
}
*/

/* As compiler adds epilogue and postlogue (addtional instruction) we can not directly call regular function to use
 * SP to print stack content. Hence using actual handler to call another function which will use to print the stack
 * Note: Using naked attribute to avoid epilog.
 * Note: As per ARM procedure call, r0 is used as argument while calling callee function, we are leveraging the same
 * in below implementation.
 *
 */
__attribute__ ((naked)) void UsageFault_Handler(void) {

	__asm ("MRS r0, MSP");                // copy value of Main SP to r0
	__asm ("B usageFaultHandler_c");      // branch to usageFaultHandler_c(r0)
}

void usageFaultHandler_c(uint32_t *pSP) {
	volatile uint32_t *pUFSR = (volatile uint32_t *) 0xE000ED2A;      // usage fault status register
	printf("Value of Usage Fault Status Register: %x\n", (unsigned int)(*pUFSR & 0xFFFF));           // prints 1: UNDEFINSTR (undefined instruction)

	printf("Stack frame content:\n");
	printf("SP = %p\n", pSP);
	printf("r0 = %lx\n", pSP[0]);        // r0 - r3 used as argument to function while calling
	printf("r1 = %lx\n", pSP[1]);
	printf("r2 = %lx\n", pSP[2]);
	printf("r3 = %lx\n", pSP[3]);
	printf("r12 = %lx\n", pSP[4]);
	printf("LR = %lx\n", pSP[5]);		// return address before exception occur (look into dis-assembly and its address of for(;;)
	printf("PC = %lx\n", pSP[6]);       // In this case it point to 0x20010000 which contained invalid instruction
	printf("XPSR = %lx\n", pSP[7]);

	while(1);  // loop forever
}

