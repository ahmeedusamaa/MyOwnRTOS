/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
#warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

#include "Stm32_F103x6.h"
#include "Stm32_F103C6_gpio_driver.h"
#include "Stm32_F103C6_EXTI_driver.h"
#include "Stm32_F103C6_USART_driver.h"
#include "lcd_driver.h"
#include "keypad_driver.h"
#include "Stm32_F103C6_SPI_driver.h"
#include "Stm32_F103C6_I2C_driver.h"
#include "I2C_Slave_EEPROM.h"
#include "TIM.h"
#include "core_cm3.h"

#include "Scheduler.h"


/*================================================================
* 						 Test Case
* 					   - Round_robin
*==================================================================*/

/*
Task_ref T1, T2, T3, T4;
unsigned char T1_Led, T2_Led, T3_Led, T4_Led;
void Task1()
{
	while(1)
	{
		T1_Led ^=1;
	}
}

void Task2()
{
	while(1)
	{
		T2_Led ^=1;
	}
}

void Task3()
{
	while(1)
	{
		T3_Led ^=1;
	}
}

int main(void)
{
	AORTOS_errorID error;
	Hardware_init();
	AORTOS_init();

	T1.Stack_Size = 512;
	T1.P_TaskEntery = Task1;
	T1.Priority = 3;
	T1.Task_State = Suspended;
	strcpy(T1.Task_name,"task1");

	T2.Stack_Size = 512;
	T2.P_TaskEntery = Task2;
	T2.Priority = 3;
	T2.Task_State = Suspended;
	strcpy(T2.Task_name,"task2");

	T3.Stack_Size = 512;
	T3.P_TaskEntery = Task3;
	T3.Priority = 3;
	T3.Task_State = Suspended;
	strcpy(T3.Task_name,"task3");



	AORTOS_CreateTask(&T1);
	AORTOS_CreateTask(&T2);
	AORTOS_CreateTask(&T3);


	AORTOS_ActivateTask(&T1);
	AORTOS_ActivateTask(&T2);
	AORTOS_ActivateTask(&T3);

	AORTOS_StartOS();

	while(1)
	{

	}
}
*/


/*================================================================
* 						 Test Case
* 				- priority_Inheritance_Solution
*==================================================================*/
/*
Task_ref T1, T2, T3, T4;
unsigned char T1_Led, T2_Led, T3_Led, T4_Led;
Mutex_ref Mutex1, Mutex2;

void Task1()
{
	static int count=0;
	while(1)
	{
		T1_Led ^=1;
		count++;
		if(count == 100)
		{
			AORTOS_Acquire_Mutex(&Mutex1, &T1);
			AORTOS_ActivateTask(&T2);
		}
		else if(count == 200)
		{
			count = 0;
			AORTOS_Release_Mutex(&Mutex1, &T1);
		}
	}
}

void Task2()
{
	static int count=0;
	while(1)
	{
		T2_Led ^=1;
		count++;
		if(count == 100)
		{
			AORTOS_ActivateTask(&T3);
		}
		else if(count == 200)
		{
			count = 0;
			AORTOS_TerminalTask(&T2);
		}
	}
}

void Task3()
{
	static int count=0;
	while(1)
	{
		T3_Led ^=1;
		count++;
		if(count == 100)
		{
			AORTOS_ActivateTask(&T4);
		}
		else if(count == 200)
		{
			count = 0;
			AORTOS_TerminalTask(&T3);
		}
	}
}

void Task4()
{
	static int count=0;
	while(1)
	{
		T4_Led ^=1;
		count++;
		if(count == 3)
		{
			AORTOS_Acquire_Mutex(&Mutex1, &T4);
		}
		if(count == 200)
		{
			count = 0;
			AORTOS_Release_Mutex(&Mutex1, &T4);
			AORTOS_TerminalTask(&T4);
		}
	}
}

int main(void)
{
	Hardware_init();
	AORTOS_init();

	strcpy(Mutex1.Mutex_Name, "MUTEX_1");
	Mutex1.PI.PI_State = Priority_Inheritance_Enable;

	strcpy(Mutex2.Mutex_Name, "MUTEX_2");
	Mutex2.PI.PI_State = Priority_Inheritance_Enable;

	T1.Stack_Size = 1024;
	T1.P_TaskEntery = Task1;
	T1.Priority = 4;
	T1.Task_State = Suspended;
	strcpy(T1.Task_name,"task1");

	T2.Stack_Size = 1024;
	T2.P_TaskEntery = Task2;
	T2.Priority = 3;
	T2.Task_State = Suspended;
	strcpy(T2.Task_name,"task2");

	T3.Stack_Size = 1024;
	T3.P_TaskEntery = Task3;
	T3.Priority = 2;
	T3.Task_State = Suspended;
	strcpy(T3.Task_name,"task3");


	T4.Stack_Size = 1024;
	T4.P_TaskEntery = Task4;
	T4.Priority = 1;
	T4.Task_State = Suspended;
	strcpy(T4.Task_name,"task4");

	AORTOS_CreateTask(&T1);
	AORTOS_CreateTask(&T2);
	AORTOS_CreateTask(&T3);
	AORTOS_CreateTask(&T4);


	AORTOS_ActivateTask(&T1);

	AORTOS_StartOS();

	while(1)
	{

	}
}
*/





/*================================================================
* 						 Test Case
* 					- Deadlock_Solution
*==================================================================*/

Task_ref T1, T2, T3, T4;
unsigned char T1_Led, T2_Led, T3_Led, T4_Led;
Mutex_ref Mutex1, Mutex2;

void Task1()
{
	static int count=0;
	while(1)
	{
		T1_Led ^=1;
		count++;
		if(count == 100)
		{
			AORTOS_Acquire_Mutex(&Mutex1, &T1);
			AORTOS_ActivateTask(&T4);
			AORTOS_Acquire_Mutex(&Mutex2, &T1);
		}
		else if(count == 200)
		{
			count = 0;
			AORTOS_Release_Mutex(&Mutex1, &T1);
		}
	}
}

void Task2()
{
	static int count=0;
	while(1)
	{
		T2_Led ^=1;
		count++;
		if(count == 100)
		{
			AORTOS_ActivateTask(&T3);
		}
		else if(count == 200)
		{
			count = 0;
			AORTOS_TerminalTask(&T2);
		}
	}
}

void Task3()
{
	static int count=0;
	while(1)
	{
		T3_Led ^=1;
		count++;
		if(count == 100)
		{
			AORTOS_ActivateTask(&T4);
		}
		else if(count == 200)
		{
			count = 0;
			AORTOS_TerminalTask(&T3);
		}
	}
}

void Task4()
{
	static int count=0;
	while(1)
	{
		T4_Led ^=1;
		count++;

		if(count == 3)
		{
			AORTOS_Acquire_Mutex(&Mutex2, &T4);
			AORTOS_Acquire_Mutex(&Mutex1, &T4);
		}
		if(count == 200)
		{
			count = 0;
			AORTOS_Release_Mutex(&Mutex1, &T4);
			AORTOS_TerminalTask(&T4);
		}
	}
}

int main(void)
{
	Hardware_init();
	AORTOS_init();

	strcpy(Mutex1.Mutex_Name, "MUTEX_1");
	Mutex1.PI.PI_State = Priority_Inheritance_Enable;

	strcpy(Mutex2.Mutex_Name, "MUTEX_2");
	Mutex2.PI.PI_State = Priority_Inheritance_Enable;

	T1.Stack_Size = 1024;
	T1.P_TaskEntery = Task1;
	T1.Priority = 4;
	T1.Task_State = Suspended;
	strcpy(T1.Task_name,"task1");

	T2.Stack_Size = 1024;
	T2.P_TaskEntery = Task2;
	T2.Priority = 3;
	T2.Task_State = Suspended;
	strcpy(T2.Task_name,"task2");

	T3.Stack_Size = 1024;
	T3.P_TaskEntery = Task3;
	T3.Priority = 2;
	T3.Task_State = Suspended;
	strcpy(T3.Task_name,"task3");


	T4.Stack_Size = 1024;
	T4.P_TaskEntery = Task4;
	T4.Priority = 1;
	T4.Task_State = Suspended;
	strcpy(T4.Task_name,"task4");

	AORTOS_CreateTask(&T1);
	AORTOS_CreateTask(&T2);
	AORTOS_CreateTask(&T3);
	AORTOS_CreateTask(&T4);


	AORTOS_ActivateTask(&T1);

	AORTOS_StartOS();

	while(1)
	{

	}
}

