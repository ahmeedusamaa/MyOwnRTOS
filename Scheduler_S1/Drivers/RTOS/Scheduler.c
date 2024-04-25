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

typedef enum {
	OSSuspended,
	OSActive
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

void OS_SVC_services(int *StackFramePointer)
{
	unsigned char SVC_number;
	SVC_number = *( (unsigned char*) ( (unsigned char*)StackFramePointer[6] )-2 ) ;
	switch(SVC_number)
	{
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	}
}

int OS_SVC_Set(int a, int b, int SVC_ID)
{
	int result;
	switch(SVC_ID)
	{
	case 1:
		__asm("svc 0x01");
		break;
	case 2:
		__asm("svc 0x02");
		break;
	case 3:
		__asm("svc 0x03");
		break;
	}

	__asm("mov %0, r0"
			:"=r" (result));
	return result;

}

void Idle_Task()
{
	while(1)
	{
		__asm("NOP");
	}
}

RTOS_errorID RTOS_Create_MainStack()
{
	RTOS_errorID error = NOError;

	// 3 KiloByte for MSP
	OS_Control._S_MSP = &_estack ;
	OS_Control._E_MSP = OS_Control._S_MSP - MainStackSize;
	OS_Control.PSP_TaskLocator = (OS_Control._E_MSP - 8);

	return error;

}

RTOS_errorID RTOS_init()
{
	RTOS_errorID error = NOError;

	//Update OS mode
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
	 * ======
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
	 *r5, r6 , r7 ,r8 ,r9, r10,r11 (Saved/Restore)Manual
	 */

	T_ref->Current_PSP = T_ref->_S_PSP_Task;

	//DUMMY_XPSR should T =1
	T_ref->Current_PSP--;
	*(T_ref->Current_PSP) = 0x1000000;

	//PC
	T_ref->Current_PSP--;
	*(T_ref->Current_PSP) =  (unsigned int)T_ref->P_TaskEntery;

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








