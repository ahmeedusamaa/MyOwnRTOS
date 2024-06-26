/*
 * Scheduler.c
 *
 *  Created on: Apr 23, 2024
 *      Author: ahmed osama
 */

#include "Scheduler.h"
#include "MYRTOS_FIFO.h"


Task_ref RTOS_IdleTask;
FIFO_Buf_t Ready_QUEUE ;
Task_ref* Ready_QUEUE_FIFO[100] ;
unsigned char re_evaluatedPriority;
#define NumOfAquiredTasks	50
Task_ref* AquiredTasks[NumOfAquiredTasks];

typedef enum {
	OSSuspended,
	OSRunning
}OS_Modes;



struct{
	Task_ref* OS_Tasks[100];
	unsigned int _S_MSP;
	unsigned int _E_MSP;
	unsigned int  PSP_TaskLocator;
	unsigned char NumberOfCreatedTask;
	Task_ref* Current_Task;
	Task_ref* Next_Task;
	OS_Modes OSModes;
}OS_Control;

unsigned char IdleTaskLed;
void Idle_Task()
{
	while(1)
	{
		IdleTaskLed ^= 1;
		/*
		 * Enter Sleep Mode "Wait For Event"
		 * if all Tasks terminated
		 */
		__asm("wfe");
	}
}

RTOS_errorID RTOS_Create_MainStack()
{
	RTOS_errorID error = NOError;
	// 3 KiloByte for MSP
	OS_Control._S_MSP = &_estack ;
	OS_Control._E_MSP = OS_Control._S_MSP - MainStackSize;
	//Align 8 byte between MSP and PSP
	OS_Control.PSP_TaskLocator = (OS_Control._E_MSP - 8);

	return error;

}

RTOS_errorID RTOS_init()
{
	RTOS_errorID error = NOError;

	/*
	 * Set OS mode Suspend
	 * Specify the MAIN stack for OS
	 * Initiate Queue
	 * Configure IDLE TASK
	 */
	OS_Control.OSModes = OSSuspended;

	//Specify the MAIN stack for OS
	RTOS_Create_MainStack();

	//Create OS ready Queue
	if (FIFO_init(&Ready_QUEUE, Ready_QUEUE_FIFO, 100) !=FIFO_NO_ERROR)
	{
		error += Ready_Queue_init_error ;
	}

	//Configure IDLE TASK
	strcpy(RTOS_IdleTask.Task_name, "idleTask");
	RTOS_IdleTask.Priority = 255;                	//lowest Priority (unsigned char -> 255)
	RTOS_IdleTask.P_TaskEntery = Idle_Task;
	RTOS_IdleTask.Stack_Size = 300;

	error += RTOS_CreateTask(&RTOS_IdleTask);

	return error;
}

RTOS_errorID RTOS_Create_TaskStack(Task_ref* T_ref)
{
	RTOS_errorID error = NOError;


	/*Task Frame
	 *  add the initial values When creating a task for the first time.
	 * ======
	 * this context is saved and restored on stack by CPU (when enter/exit interrupt mode):
	 * XPSR
	 * PC (Next Task Instruction which should be Run)
	 * LR (return register which is saved in CPU while TASk1 running before TaskSwitching)
	 * r12
	 * r4
	 * r3
	 * r2
	 * r1
	 * r0
	 *====
	 *will push and pop manually the addition frame to save the other general purpose registers:
	 *r5, r6 , r7 ,r8 ,r9, r10,r11 (Saved/Restore)Manual
	 */

	T_ref->Current_PSP = T_ref->_S_PSP_Task;

	/*
	 * DUMMY_XPSR should T =1
	 * Decrease Current PSP value
	 */
	T_ref->Current_PSP--;
	*(T_ref->Current_PSP) = 0x1000000;

	//PC
	T_ref->Current_PSP--;
	*(T_ref->Current_PSP) =  (unsigned int)T_ref->P_TaskEntery;  				//address of the Task to start execution

	//LR
	T_ref->Current_PSP--;
	//LR = 0xFFFFFFFD (EXC_RETURN)Return to thread with PSP
	*(T_ref->Current_PSP) = 0xFFFFFFFD;

	int i;
	for(i=0 ; i<13 ; i++ )
	{
		//rx
		T_ref->Current_PSP--;
		*(T_ref->Current_PSP) = 0;
	}

	return error;
}

RTOS_errorID RTOS_CreateTask(Task_ref* T_ref)
{
	RTOS_errorID error = NOError;

	//create Its own PSP stack
	//check task stack size exceeded the PSP stack
	T_ref->_S_PSP_Task =  OS_Control.PSP_TaskLocator;
	T_ref->_E_PSP_Task = ( OS_Control.PSP_TaskLocator - T_ref->Stack_Size );

	//	-				-
	//	- _S_PSP_Task	-
	//	-	Task Stack	-
	//	- _E_PSP_Task	-
	//	-				-
	//	- _eheap		-
	//	-				-
	//

	if(T_ref->_E_PSP_Task < (unsigned int)(&(_eheap)))
	{
		//return Task_exceeded_StackSize;
	}

	//Aligned 8 Bytes spaces between Task PSP and other
	OS_Control.PSP_TaskLocator =  (T_ref->_E_PSP_Task - 8);

	//Initialize PSP Task Stack
	RTOS_Create_TaskStack(T_ref);

	//update sch Table
	OS_Control.OS_Tasks[OS_Control.NumberOfCreatedTask] = T_ref;
	OS_Control.NumberOfCreatedTask++;

	//Task state Update suspend
	T_ref->Task_State = Suspended;

	return error;
}

void BubbleSort_SchedulerTable()
{
	Task_ref* Temp;
	int i,j;
	for(i=0; i< (OS_Control.NumberOfCreatedTask - 1); i++)
		for(j=0; j< (OS_Control.NumberOfCreatedTask - 1 -i); j++)
		{
			if(OS_Control.OS_Tasks[j]->Priority > OS_Control.OS_Tasks[j+1]->Priority)
			{
				Temp = OS_Control.OS_Tasks[j];
				OS_Control.OS_Tasks[j]  = OS_Control.OS_Tasks[j+1];
				OS_Control.OS_Tasks[j+1]  = Temp;
			}
		}

}

//Handler mode
void RTOS_Update_SchedulerTable()
{
	BubbleSort_SchedulerTable();

	Task_ref* Temp = NULL;
	//Free Ready Queue
	while(FIFO_dequeue(&Ready_QUEUE, &Temp)!=FIFO_EMPTY);

	//Update Ready Queue
	Task_ref* Ptask;
	Task_ref* PnextTask;
	int i=0;
	while(i<OS_Control.NumberOfCreatedTask)
	{
		Ptask = OS_Control.OS_Tasks[i];
		PnextTask = OS_Control.OS_Tasks[i+1];
		if(Ptask->Task_State != Suspended)
		{
			// Check Priority:

			if(PnextTask->Task_State == Suspended && PnextTask == OS_Control.OS_Tasks[OS_Control.NumberOfCreatedTask -1] )
			{
				/*
				 * Last task Suspended
				 * Enqueue Current Task
				 */
				FIFO_enqueue(&Ready_QUEUE, Ptask);
				Ptask->Task_State=Ready;
				break;
			}

			if(Ptask->Priority < PnextTask->Priority )
			{
				// Enqueue Current Task (highest priority)
				FIFO_enqueue(&Ready_QUEUE, Ptask);
				Ptask->Task_State=Ready;
				break;
			}

			else if(Ptask->Priority == PnextTask->Priority )
			{
				/*
				 * Round-Robin
				 * Enqueue Current Task
				 * Keep the loop running to continuously enqueue tasks, Round-Robin
				 */
				FIFO_enqueue(&Ready_QUEUE, Ptask);
				Ptask->Task_State=Ready;
			}
		}
		i++;
	}
	if(i == OS_Control.NumberOfCreatedTask)
	{
		if(Ptask->Task_State != Ready && ( PnextTask->Task_State != Ready || PnextTask == NULL ))
		{
			Ptask = OS_Control.OS_Tasks[OS_Control.NumberOfCreatedTask-1];
			FIFO_enqueue(&Ready_QUEUE, Ptask);
			Ptask->Task_State=Ready;
		}
	}
}

typedef enum{
	ActivateTask,
	TerminateTask,
	TaskWaitingTime,
	AcquireMutex,
	ReleaseMutex
}SVC_ID;


void Decide_whatNext()
{
	/*
	 * current task is set to the running state and added back to the ready queue
	 * round-robin scheduling algorithm
	 */
	if(Ready_QUEUE.counter == 0 && OS_Control.Current_Task->Task_State != Suspended)
	{
		OS_Control.Current_Task->Task_State = Running;
		FIFO_enqueue(&Ready_QUEUE, OS_Control.Current_Task);

		/* This line is crucial for the next step, as it determines which task's
		 * information will be saved and which task will later restore its context.
		 */
		OS_Control.Next_Task = OS_Control.Current_Task;
	}else
	{
		/*
		 * next task is dequeued from the ready queue using the FIFO_dequeue function and set to the running state.
		 */
		FIFO_dequeue(&Ready_QUEUE, &OS_Control.Next_Task);
		OS_Control.Next_Task->Task_State = Running;


		/*
		 * added back to the ready queue and its state is set to ready
		 */
		if((OS_Control.Current_Task->Priority == OS_Control.Next_Task->Priority ) && ( OS_Control.Current_Task->Task_State != Suspended))
		{
			FIFO_enqueue(&Ready_QUEUE, OS_Control.Current_Task);
			OS_Control.Current_Task->Task_State = Ready;
		}
	}
}

__attribute ((naked)) PendSV_Handler()
																																																														{
	/*
	 * Save Context of current task .
	 * update the current task by the next task to be executed.
	 * load the context of the task which is about to execute.
	 */

	/*
	 * Save the Context of the Current Task?
	 * Automatically saved by the CPU when entering Handler mode.
	 * XPSR
	 * PC (Next Task Instruction which should be Run)
	 * LR (return register which is saved in CPU while TASk1 running before TaskSwitching)
	 * r12
	 * r4
	 * r3
	 * r2
	 * r1
	 * r0
	 */

	/*
	 * Get the Current Task "Current PSP"
	 * manually save the addition frame of other general purpose registers
	 * r5, r6 , r7 ,r8 ,r9, r10,r11
	 */
	OS_GET_PSP(OS_Control.Current_Task->Current_PSP);        //Current_Task for OS when OS in running mode

	OS_Control.Current_Task->Current_PSP--;
	__asm volatile("mov %0, r4 \n\t" : "=r" (*OS_Control.Current_Task->Current_PSP) );
	OS_Control.Current_Task->Current_PSP--;
	__asm volatile("mov %0, r5 \n\t" : "=r" (*OS_Control.Current_Task->Current_PSP) );
	OS_Control.Current_Task->Current_PSP--;
	__asm volatile("mov %0, r6 \n\t" : "=r" (*OS_Control.Current_Task->Current_PSP) );
	OS_Control.Current_Task->Current_PSP--;
	__asm volatile("mov %0, r7 \n\t" : "=r" (*OS_Control.Current_Task->Current_PSP) );
	OS_Control.Current_Task->Current_PSP--;
	__asm volatile("mov %0, r8 \n\t" : "=r" (*OS_Control.Current_Task->Current_PSP) );
	OS_Control.Current_Task->Current_PSP--;
	__asm volatile("mov %0, r9 \n\t" : "=r" (*OS_Control.Current_Task->Current_PSP) );
	OS_Control.Current_Task->Current_PSP--;
	__asm volatile("mov %0, r10 \n\t" : "=r" (*OS_Control.Current_Task->Current_PSP) );
	OS_Control.Current_Task->Current_PSP--;
	__asm volatile("mov %0, r11 \n\t" : "=r" (*OS_Control.Current_Task->Current_PSP) );

	/*
	 * Restore the Context of the next Task
	 * will restore manually frame of general purpose registers
	 * as these will not be automatically restored by the CPU.
	 */

	/*
	 * 	OS_SET_PSP(OS_Control.Next_Task->Current_PSP);
	 * 	This step After pushing the general purpose registers onto the stack manually,
	 * 	CPU PSP register will point to r0
	 */

	/*===========================================================================================
 	    /* If the next task is NULL, this line sets the current task as the IDLE TASK
	 * of the OS (which is the number of tasks - 1
	 * as the idle task has the lowest priority),
	 * Without it, if the next task is NULL, the context switch operation risks
	 * attempting to restore garbage values, including the Program Counter (PC)
	 * That could lead to unpredictable behavior, , potentially jumping to an
	 * unknown memory location.

		if (OS_Control.Next_Task == NULL){
			OS_Control.Current_Task = OS_Control.OS_Tasks[OS_Control.NumberOfCreatedTask-1] ;
			OS_Control.Next_Task = NULL ;
		}else{
			OS_Control.Current_Task = OS_Control.Next_Task ;
			OS_Control.Next_Task = NULL ;
		}
	==============================================================================================
	 */

	/*
	 * Without the next condition, if the next task is NULL, the context switch operation risks
	 * attempting to restore garbage values, including the Program Counter (PC)
	 * That could lead to unpredictable behavior, , potentially jumping to an
	 * unknown memory location.
	 */
	if (OS_Control.Next_Task != NULL){
		OS_Control.Current_Task = OS_Control.Next_Task ;
		OS_Control.Next_Task = NULL ;
	}

	__asm volatile("mov r11, %0" : : "r" (*OS_Control.Current_Task->Current_PSP) );
	OS_Control.Current_Task->Current_PSP++;
	__asm volatile("mov r10, %0" : : "r" (*OS_Control.Current_Task->Current_PSP) );
	OS_Control.Current_Task->Current_PSP++;
	__asm volatile("mov r9, %0"  : : "r" (*OS_Control.Current_Task->Current_PSP) );
	OS_Control.Current_Task->Current_PSP++;
	__asm volatile("mov r8, %0"  : : "r" (*OS_Control.Current_Task->Current_PSP) );
	OS_Control.Current_Task->Current_PSP++;
	__asm volatile("mov r7, %0"  : : "r" (*OS_Control.Current_Task->Current_PSP) );
	OS_Control.Current_Task->Current_PSP++;
	__asm volatile("mov r6, %0"  : : "r" (*OS_Control.Current_Task->Current_PSP) );
	OS_Control.Current_Task->Current_PSP++;
	__asm volatile("mov r5, %0"  : : "r" (*OS_Control.Current_Task->Current_PSP) );
	OS_Control.Current_Task->Current_PSP++;
	__asm volatile("mov r4, %0"  : : "r" (*OS_Control.Current_Task->Current_PSP) );
	OS_Control.Current_Task->Current_PSP++;

	/*
	 * Update PSP register
	 * XPSR
	 * PC (Next Task Instruction which should be Run)
	 * LR (return register which is saved in CPU while TASk1 running before TaskSwitching)
	 * r12
	 * r4
	 * r3
	 * r2
	 * r1
	 * r0
	 * Automatically restored by the CPU after ("BX LR").
	 */
	OS_SET_PSP(OS_Control.Current_Task->Current_PSP);
	/*
	 * __attribute ((naked)) means no stack frame set up by the compiler
	 * manually handle the exit from this Handler
	 * as the compiler will not automatically generate the exit code.
	 */
	__asm volatile("BX LR");
																																																														}

//Handler Mode
void OS_SVC(int *StackFramePointer)
{
	/*
	 * Stack contains:
	 * r0, r1, r2, r3, r12, LR, PC and xPSR
	 * First argument (r0) is svc_args[0]
	 */
	unsigned char SVC_number;
	SVC_number = *( (unsigned char*) ( (unsigned char*)StackFramePointer[6] )-2 ) ;
	switch(SVC_number)
	{
	case ActivateTask:
	case TerminateTask:
	case AcquireMutex:
	case ReleaseMutex:
		//Update Sch table, Ready Queue
		RTOS_Update_SchedulerTable();
		//OS is in Running State
		if (OS_Control.OSModes == OSRunning)
		{
			//
			if(strcmp(OS_Control.Current_Task->Task_name,"idleTask") != 0)
			{
				//Decide what next
				Decide_whatNext();
				//Context Switching
				trigger_OS_PendSV();
			}
		}
		break;
	case TaskWaitingTime:
		RTOS_Update_SchedulerTable();
		break;
	}
}

//Thread Mode
int OS_SVC_Set(SVC_ID ID)
{
	switch(ID)
	{
	case ActivateTask:
		__asm("svc #0x00");
		break;
	case TerminateTask:
		__asm("svc #0x01");
		break;
	case TaskWaitingTime:
		__asm("svc #0x02");
		break;
	case AcquireMutex:
		__asm("svc #0x03");
	case ReleaseMutex:
		__asm("svc #0x04");

	}
}


void RTOS_ActivateTask(Task_ref* T_ref)
{
	T_ref->Task_State = Waiting;
	OS_SVC_Set(ActivateTask);
}

void RTOS_TerminalTask(Task_ref* T_ref)
{
	T_ref->Task_State = Suspended;
	OS_SVC_Set(TerminateTask);
}

void RTOS_StartOS()
{
	OS_Control.OSModes = OSRunning;
	//set the default task to idle task
	OS_Control.Current_Task = &RTOS_IdleTask;
	//Activate Idle task
	RTOS_ActivateTask(&RTOS_IdleTask);

	//start Systick
	Start_Systick(); //1ms

	OS_SET_PSP(OS_Control.Current_Task->Current_PSP);
	//Switch tread mode change SP from MSP to PSP
	OS_SWITCH_SP_to_PSP;
	SWITCH_CPU_AccessLevel_unprivileged;

	RTOS_IdleTask.P_TaskEntery();
}

void RTOS_TaskWait(unsigned int ticks, Task_ref* T_ref)
{
	T_ref->TimeWait.Blocking= Blocking_Enable;
	T_ref->TimeWait.Ticks_count = ticks;
	RTOS_TerminalTask(T_ref);

}

void RTOS_Update_TaskWaitingTime()
{
	for(int i=0; i<OS_Control.NumberOfCreatedTask; i++)
	{
		if(OS_Control.OS_Tasks[i]->Task_State == Suspended)
		{
			if(OS_Control.OS_Tasks[i]->TimeWait.Blocking == Blocking_Enable)
			{
				OS_Control.OS_Tasks[i]->TimeWait.Ticks_count--;
				if(	OS_Control.OS_Tasks[i]->TimeWait.Ticks_count == 0)
				{
					OS_Control.OS_Tasks[i]->TimeWait.Blocking= Blocking_Disable;
					OS_Control.OS_Tasks[i]->Task_State = Waiting;
					OS_SVC_Set(TaskWaitingTime);

				}
			}
		}
	}
}


/*
 * When requesting the MUTEX, if the task cannot obtain it, the priority of the task that owns the MUTEX must be re-evaluated.
 */

void RTOS_Acquire_Mutex(Mutex_ref *Acquired_Mutex, Task_ref *T_ref)
{
	if(Acquired_Mutex->Current_Task == NULL)
	{
		Acquired_Mutex->Current_Task = T_ref;
		//Add task to array for acquired tasks
		for(int i=0;i<NumOfAquiredTasks;i++){
			if(AquiredTasks[i]== NULL){
				AquiredTasks[i] = T_ref ;
				break;
			}
		}
	}else
	{
		int i = 0;
		if(Acquired_Mutex->Next_Task == NULL)
		{
			//Deadlock avoidance by Stopping a task from Aqiring more than one Mutex
			for(i=0;i<NumOfAquiredTasks;i++){
				//Check if the next task is already Acquiring one or not
				if(T_ref == AquiredTasks[i]){

					break;

				}
			}
			if(i == NumOfAquiredTasks)
			{
				Acquired_Mutex->Next_Task = T_ref;
				T_ref->Task_State = Suspended;
				if(Acquired_Mutex->PI.PI_State == Priority_Inheritance_Enable)
				{
					Acquired_Mutex->PI.CurrentTask_Priority = Acquired_Mutex->Current_Task->Priority;
					Acquired_Mutex->Current_Task->Priority = T_ref->Priority;
				}
				//Add task to array for acquired tasks
				for(int i=0;i<NumOfAquiredTasks;i++){
					if(AquiredTasks[i]== NULL){
						AquiredTasks[i] = T_ref ;
						break;
					}
				}
				OS_SVC_Set(AcquireMutex);
			}
		}
	}
}

void RTOS_Release_Mutex(Mutex_ref *Release_Mutex, Task_ref *T_ref)
{
	if(Release_Mutex->Current_Task != NULL)
	{
		//Remove task from array for acquired tasks
		for(int i=0;i<NumOfAquiredTasks;i++){
			if(AquiredTasks[i] == OS_Control.Current_Task ){
				AquiredTasks[i]= NULL;
				break;
			}
		}
		if(Release_Mutex->Current_Task == T_ref)
		{
			if(Release_Mutex->PI.PI_State == Priority_Inheritance_Enable)
			{
				T_ref->Priority = Release_Mutex->PI.CurrentTask_Priority;
				Release_Mutex->PI.CurrentTask_Priority = Release_Mutex->Next_Task->Priority;
			}
			if(Release_Mutex->Next_Task == NULL)
			{
				Release_Mutex->Current_Task = NULL;
			}else{
				Release_Mutex->Current_Task = Release_Mutex->Next_Task;
				Release_Mutex->Next_Task = NULL;
				Release_Mutex->Current_Task->Task_State = Ready;
				OS_SVC_Set(ReleaseMutex);
			}
		}
	}
}
