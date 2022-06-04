/*
 * taskdef.h
 *
 *  Created on: Jan 20, 2021
 *      Author: florentgoutailler
 */

#ifndef TASKDEF_H_
#define TASKDEF_H_

//extern variable
extern uint32_t RTOS_RunTimeCounter;

//Tasks

void vInitTask(void *);
void vTask1a(void *);
void vTask1b(void *);
void vTask2a(void *);
void vTask2b(void *);
void vTask3(void *);
void vTask4(void *);
#endif /* TASKDEF_H_ */
