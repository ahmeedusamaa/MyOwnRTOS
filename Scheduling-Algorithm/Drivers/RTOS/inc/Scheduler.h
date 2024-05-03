/*
 * Scheduler.h
 *
 *  Created on: Apr 23, 2024
 *      Author: ahmed osama
 */

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

#include "CortexMX_OS_porting.h"

typedef enum{
	Suspended,
	Ready,
	Waiting,
	Running
}T_State;

typedef struct{
	enum{
		Blocking_Enable,
		Blocking_Disable
	}Blocking;
	unsigned int Ticks_count;
}Time_wait;

typedef struct{

	unsigned int Stack_Size;
	unsigned char Priority;
	void (*P_TaskEntery)(void);
	unsigned int _S_PSP_Task;
	unsigned int _E_PSP_Task;
	unsigned int* Current_PSP;
	T_State Task_State;
	char Task_name[30];
	Time_wait TimeWait;

}Task_ref;

typedef struct{
	unsigned char* Ppayload;
	unsigned int PayloadSize;
	Task_ref* CurrentTaskUser;
	Task_ref* NexttTaskUser;
}Mutex_ref;

typedef enum {
	NOError,
	Ready_Queue_init_error,
	Task_exceeded_StackSize,
	MutexMaxNumberOfUsers
}RTOS_errorID;

RTOS_errorID RTOS_init();
RTOS_errorID RTOS_CreateTask(Task_ref *Task_ref);
void RTOS_ActivateTask(Task_ref* T_ref);
void RTOS_TerminalTask(Task_ref* T_ref);
void Decide_whatNext();
void RTOS_StartOS();
void RTOS_TaskWait(unsigned int ticks, Task_ref* T_ref);
void RTOS_Update_TaskWaitingTime();

#endif /* INC_SCHEDULER_H_ */
