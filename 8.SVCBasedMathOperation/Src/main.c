/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Sunil Kumar Yadav
 * @brief          : Example program to understand how to use SVCall exception
 * can be leveraged to performed privileged task in OS environment.
 *
 * In this exercise we will perform basic math operation on behalf of user(thread mode)
 * program where SVC no. associated with specific operation. Below are SVC no. for
 * math operation used:
 * 20: Addition
 * 30: Subtraction
 * 40: Multiplication
 * 50: Division
 * Other: Invalid
 * Note: Basic math operation without considering corner cases
 *
 ******************************************************************************
 * Note: Better approach is to have PendSVC instruction rather than direct SVC.
 *
 ******************************************************************************
 */

#include <stdint.h>
#include <stdio.h>

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

/* To avoid issues due to epilog/prolog while jumping to SVC_Handler exception handler we are
 * using this function to print stack trace and actual SVC_Handler (naked function)
 * to simply call this function to print required stack frame data
 * */
void SVC_Handler_c(uint32_t *pMSP);

int32_t addition(int32_t x, int32_t y);
int32_t subtraction(int32_t x, int32_t y);
int32_t multiplication(int32_t x, int32_t y);
int32_t division(int32_t x, int32_t y);
//-----------------------------------------------------------------------------------------------------------------------------

int main(void)
{
	int32_t result;

	result = addition(44, -4);
	printf("addition: %ld\n", result);

	result = subtraction(88, 10);
	printf("addition: %ld\n", result);

	result = multiplication(20, 10);
	printf("addition: %ld\n", result);

	result = division(50, -5);
	printf("addition: %ld\n", result);

	for(;;);
}

//-----------------------------------------------------------------------------------------------------------------------------
int32_t addition(int32_t x, int32_t y) {
	int32_t result;
	__asm volatile("SVC #20");                        // causes SVC exception
	__asm volatile("MOV %0, R0" : "=r"(result) :: );  // collect result of SVC exception handler in r0 to c variable
	return result;
}

int32_t subtraction(int32_t x, int32_t y) {
	int32_t result;
	__asm volatile("SVC #30");                        // causes SVC exception
	__asm volatile("MOV %0, R0" : "=r"(result) :: );  // collect result of SVC exception handler in r0 to c variable
	return result;
}

int32_t multiplication(int32_t x, int32_t y) {
	int32_t result;
	__asm volatile("SVC #40");                        // causes SVC exception
	__asm volatile("MOV %0, R0" : "=r"(result) :: );  // collect result of SVC exception handler in r0 to c variable
	return result;
}

int32_t division(int32_t x, int32_t y) {
	int32_t result;
	__asm volatile("SVC #50");                        // causes SVC exception
	__asm volatile("MOV %0, R0" : "=r"(result) :: );  // collect result of SVC exception handler in r0 to c variable
	return result;
}

//-------------------------------------------------------------------------------------------------------------
/* As compiler adds epilogue and postlogue (addtional instruction) we can not directly call regular function to use
 * SP to print stack content. Hence using actual handler to call another function which will use to print the stack
 * Note: Using naked attribute to avoid epilog.
 * Note: As per ARM procedure call, r0-r3 is used as argument while calling callee function and callee function use r0,r1 to return value to calling function. We are leveraging the same
 * in below implementation.
 *
 */
__attribute__ ((naked)) void SVC_Handler(void) {

	__asm ("MRS r0, MSP");                // copy value of Main SP to r0
	__asm ("B SVC_Handler_c");            // branch to SVC_Handler_c(r0, r1) as r0-r3 used as function input arg
}

/* Based on SVC number in SVCall exception function perform mathematical operation.
 */
void SVC_Handler_c(uint32_t *pSP) {
	// pSP -> Base of stack frame

	printf("Inside SVC Exception Handler\n");

	int32_t arg0, arg1, result;

	uint8_t* pReturnAddress = (uint8_t*)pSP[6];     // Points to PC and decrementing by few values will give us location to
													// SVC instruction opcode. Note as Thumb have 16bit instruction, we are using  uint8_t* pointer

	pReturnAddress -= 2;                            // SVC instruction address

	// extracting svc no. from LSB byte of OpCode at address pointed by pReturnAddress
	uint8_t svcNumber = *pReturnAddress;
	//printf("SVC no. received from thread mode: %d\n",svcNumber);

	arg0 = pSP[0];        							// r0
	arg1 = pSP[1];        							// r1

	switch (svcNumber) {
		case 20:
				result = arg0 + arg1;
				break;
		case 30:
				result = arg0 - arg1;
				break;
		case 40:
				result = arg0 * arg1;
				break;
		case 50:
				result = arg0 / arg1;
				break;
		default:
			printf("Invalid SVC code\n");

	}

	// Writing result of math operation into register r0 which thread mode code can retrieve
	pSP[0] = result;

	/*
    printf("r0 = %lx\n", pSP[0]);        // r0 - r3 used as argument to function while calling
	printf("r1 = %lx\n", pSP[1]);
	printf("r2 = %lx\n", pSP[2]);
	printf("r3 = %lx\n", pSP[3]);
	printf("r12 = %lx\n", pSP[4]);
	printf("LR = %lx\n", pSP[5]);		// return address before exception occur (look into dis-assembly and its address of for(;;)
	printf("PC = %lx\n", pSP[6]);       // PC point to address of nxt instruction
	printf("XPSR = %lx\n", pSP[7]);
	*/
}

/* How we extracted the value of SVC no? Below is assembly instruction excerpt.
 * PC holds address of next instruction which we got from pSP[6] (as stack model is full descending) i.e. 08000288
 * Decrementing by 2 gives address 08000286 which is SVC instruction. Opcode is 14 df. LSB is 14 here which is input 20 i.e. 0x14
 *   Address |Op Code      | ASM instruction
	--------------------------------------------
	08000280: 80 b5         push    {r7, lr}
	08000282: 82 b0         sub     sp, #8
	08000284: 00 af         add     r7, sp, #0
	08000286: 14 df         svc     20
	08000288: 03 46         mov     r3, r0
	0800028a: 7b 60         str     r3, [r7, #4]
	0800028c: 79 68         ldr     r1, [r7, #4]
	0800028e: 02 48         ldr     r0, [pc, #8]
 */

