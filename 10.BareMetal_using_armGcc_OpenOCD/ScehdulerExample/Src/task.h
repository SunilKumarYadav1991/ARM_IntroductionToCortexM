/*
 * task.h
 *
 *  Created on: 22-Nov-2022
 *      Author: Sunil Yadav
 */

#ifndef TASK_H_
#define TASK_H_
#include <stdint.h>

void taskHandler1(void);
void taskHandler2(void);
void taskHandler3(void);
void taskHandler4(void);
void idleTaskHandler(void);

/** Task control block carries private information of each task
 */
typedef struct taskControlBlock_s {
	uint32_t pspValue;
	uint32_t blockCount;
	uint8_t currentState;
	void (*taskHandlerFptr)(void);
}tcb_t;

#endif /* TASK_H_ */
