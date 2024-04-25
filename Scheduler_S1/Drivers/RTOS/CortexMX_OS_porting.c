/*
 * CortexMX_OS_porting.c
 *
 *  Created on: Apr 23, 2024
 *      Author: ahmed osama
 */

#include "CortexMX_OS_porting.h"

void HardFault_Handler()
{

}
void MemManage_Handler()
{

}
void BusFault_Handler()
{

}
void UsageFault_Handler()
{

}

void PendSV_Handler()
{
	SCB->ICSR |= SCB_ICSR_PENDSVCLR_Msk;
}


__attribute ((naked))void SVC_Handler()
{
	__asm("tst lr, #4 \n\t"
		  "ITE EQ \n\t"
		  "mrseq r0,MSP \n\t"
		  "mrsne r0,PSP \n\t"
		  "B OS_SVC_services");
}

void Hardware_init()
{
	//intialize clock tree (RCC -> SysTick Timer & CPU) 8 MHZ
	//init HW
	//Clock Tree
	//RCC Default values makes CPU Clock & SysTick Timer clock = 8 MHZ

	// 8 MHZ
	// 1 count -> 0.125 us
	// X count -> 1 ms
	// X = 8000 count
}
