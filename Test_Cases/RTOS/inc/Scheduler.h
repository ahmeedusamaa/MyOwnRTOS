/*
 * Scheduler.h
 *
 *  Created on: Apr 23, 2024
 *      Author: ahmed osama
 */

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

#include "CortexMX_OS_porting.h"

#define NumOfMutex 50

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

typedef enum
{
	Priority_Inheritance_Disable,
	Priority_Inheritance_Enable
}Priority_Inheritance_State;

typedef struct
{
	Priority_Inheritance_State PI_State;
	uint8_t CurrentTask_Priority;
}Priority_Inheritance;

typedef struct
{
	Task_ref *Current_Task;
	Task_ref *Next_Task ;
	uint8_t Mutex_Name[30];
	Priority_Inheritance PI;
}Mutex_ref;

typedef enum {
	NOError,
	Ready_Queue_init_error,
	Task_exceeded_StackSize,
	MutexMaxNumberOfUsers
}AORTOS_errorID;

/**================================================================
* @Fn				- AORTOS_init
* @brief		    - Initialize OS (create the Main stack area with user defined size)
* @param [in] 		- none
* @retval		    - Return the State of the OS According to initialization operation
* Note				- none
*==================================================================*/
AORTOS_errorID AORTOS_init();

/*================================================================
* @Fn				- AORTOS_CreateTask
* @brief		    - Create new task and design its stack
* @param [in] 		- Struct for Task information
* @retval		    - Return State
* Note				- Task in Suspend State until AORTOS_ActivateTask
*==================================================================*/
AORTOS_errorID AORTOS_CreateTask(Task_ref *Task_ref);

/*================================================================
* @Fn				- AORTOS_ActivateTask
* @brief		    - Activate specified task
* @param [in] 		- Struct for Task information
* @retval		    - none
* Note				- Task was put in Waiting State until AORTOS_Decide_whatNext
*==================================================================*/
void AORTOS_ActivateTask(Task_ref* T_ref);

/*================================================================
* @Fn				- AORTOS_TerminalTask
* @brief		    - Terminate or suspend a specified task
* @param [in] 		- Struct for Task information
* @retval		    - none
*==================================================================*/
void AORTOS_TerminalTask(Task_ref* T_ref);

/**================================================================
* @Fn				- AORTOS_Decide_whatNext
* @brief		    - round-robin scheduling algorithm
* @param [in] 		- none
* @retval		    - none
*==================================================================*/
void AORTOS_Decide_whatNext();

/**================================================================
* @Fn				- AORTOS_StaAORTOS
* @brief		    - Start Scheduling
* @param [in] 		- none
* @retval		    - none
*==================================================================*/
void AORTOS_StartOS();

/**================================================================
* @Fn				- AORTOS_TaskWait
* @brief		    - Put task in Waiting State For Specific Time
* @param [in] 		- Ticks
* @param [in] 		- Struct for Task information
* @retval		    - none
* @Note				- Waiting time in ms
*==================================================================*/
void AORTOS_TaskWait(unsigned int ticks, Task_ref* T_ref);

/**================================================================
* @Fn				- AORTOS_Update_TaskWaitingTime
* @brief		    - Update (decrease) Task Wait Time
* @param [in] 		- none
* @retval		    - none
* @Note				- Waiting time in ms
*==================================================================*/
void AORTOS_Update_TaskWaitingTime();

/**================================================================
* @Fn				- AORTOS_Acquire_Mutex
* @brief		    - Task acquire Mutex
* @param [in] 		- Struct for Mutex information
* @param [in] 		- Struct for Task information
* @retval		    - none
* @Note				- Just Two task can Take this Mutex, task cant Acquire more than one Mutex
*==================================================================*/
void AORTOS_Acquire_Mutex(Mutex_ref *Acquired_Mutex, Task_ref *Task);

/**================================================================
* @Fn				- AORTOS_Release_Mutex
* @brief		    - Task release Mutex
* @param [in] 		- Struct for Mutex information
* @param [in] 		- Struct for Task information
* @retval		    - none
*==================================================================*/
void AORTOS_Release_Mutex(Mutex_ref *Release_Mutex, Task_ref *T_ref);

#endif /* INC_SCHEDULER_H_ */
