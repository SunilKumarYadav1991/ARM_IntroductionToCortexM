/*
 * AccessLevel.c
 *
 *  Created on: 08-Nov-2022
 *      Author: Sunil Yadav
 *      @brief: This program explores access modes of Cortex-M controller. Usually used in OS environment where kernel run in
 *      privilege mode and user task in unprivileged mode.
 *      In Arm function can run either in
 *      1. un-privilege mode: has limited access to the MSR and MRS instructions,
 *      and cannot use the CPS instruction or
 *      2. privilege mode:The software can use all the instructions and has access to all resources.
 *      Privileged software executes at the privileged level.
 *
 *      Note:
 *      CONTROL Register: The CONTROL register controls the stack used and the privilege level for software execution when the processor is in thread mode and,
 *      if implemented, indicates whether the FPU state is active
 *      MSR: Move the contents of a general-purpose register into the specified special register
 *      MRS: Move the contents of a special register to a general-purpose register
 *      .
 */


void changeAccessLevelUnprivilege(void) {

	__asm volatile("MRS R0, CONTROL");    // read CONTROL register value in R0

	__asm volatile("ORR R0,R0, #0x01");   // modify/OR value of R0 with value 0x01

	__asm volatile("MSR CONTROL, R0");

}

// Note: After entering in unprivilege access level, user task can not access MSR,MRS and hence can not go back to priviledge mode.
// Only way is to change thread mode to handler mode via exception where CONTROL reg can be chaged and after exit of exception handler
// mode will be thread mode.

