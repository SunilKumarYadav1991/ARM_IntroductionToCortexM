/*
 * AssemblyEx.c
 *
 *  @Created on: 08-Nov-2022
 *  @Author: Sunil Yadav
 *  @brief: Simple program to understand how to use inline assembly function to access registers etc
 *
 */


void assembly_inline_example(void){

#if 0
	// Example assebly instructions
	__asm volatile("LDR R1, =#0x20001000");
	__asm volatile("LDR R2, =#0x20001004");
	__asm volatile("LDR R0, [R1]");
	__asm volatile("LDR R1, [R2]");
	__asm volatile("ADD R0,R0,R1");
	__asm volatile("STR R0, [R2]");
#endif

	// Example to manupulate local C variables using inline assembly

	int val = 50;
	__asm volatile("MOV R0, %0": :"r"(val));     // store value from val to R0

	int controRegister;
	__asm volatile("MRS %0, CONTROL": "=r"(controRegister));  // read CONTROL register value into local variable controRegister

	int p1, *p2;
	p2 = (int*) 0x20000008;

	__asm volatile("LDR %0, [%1]": "=r"(p1): "r"(p2));

}
