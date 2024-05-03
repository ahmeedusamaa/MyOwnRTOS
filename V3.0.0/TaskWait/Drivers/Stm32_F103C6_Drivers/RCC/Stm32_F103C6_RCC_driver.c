/*
 * Stm32_F103C6_RCC_driver.c
 *
 *  Created on: Jul 21, 2023
 *      Author: ahmed osama
 */

#include "Stm32_F103C6_RCC_driver.h"

//Bits 10:8 PPRE1[2:0]: APB Low-speed prescaler (APB1)
//Set and cleared by software to control the division factor of the APB Low speed clock (PCLK1).
//0xx: HCLK not divided
//100: HCLK divided by 2
//101: HCLK divided by 4
//110: HCLK divided by 8
//111: HCLK divided by 16
//Caution: Software must configure these bits ensure that the frequency in this domain does not
//exceed 36 MHz

const uint8_t APB1_PrescTable[8U]={0,0,0,0,1,2,3,4};  //shift right

//Bits 13:11 PPRE2[2:0]: APB high-speed prescaler (APB2)
//Set and cleared by software to control the division factor of the APB High speed clock (PCLK2).
//0xx: HCLK not divided
//100: HCLK divided by 2
//101: HCLK divided by 4
//110: HCLK divided by 8
//111: HCLK divided by 16

const uint8_t APB2_PrescTable[8U]={0,0,0,0,1,2,3,4};  //shift right

//Bits 7:4 HPRE[3:0]: AHB prescaler
//Set and cleared by software to control AHB clock division factor.
//0xxx: SYSCLK not divided
//1000: SYSCLK divided by 2
//1001: SYSCLK divided by 4
//1010: SYSCLK divided by 8
//1011: SYSCLK divided by 16
//1100: SYSCLK divided by 64
//1101: SYSCLK divided by 128
//1110: SYSCLK divided by 256
//1111: SYSCLK divided by 512
//Note: The prefetch buffer must be kept on when using a prescaler different from 1 on the AHB clock.
//Refer to the section Reading the Flash memory for more details.
//Caution: The AHB clock frequency must be at least 25 MHz when the Ethernet is used.

const uint8_t AHB_PrescTable[16U]={0,0,0,0,0,0,0,0,1,2,3,4,6,7,8,9};  //shift right

uint32_t MCAL_RCC_GetSYSFreq(void)
{
	switch ((RCC->CFGR >> 2) & 0b11)
	{
	case 0: return HSI_RC_CLK; break;
	case 1: return HSE_CLK   ; break;
	case 2: return 16000000  ; break;

	}
}
uint32_t MCAL_RCC_GetHCLFreq(void)
{
	//Bits 7:4 HPR: AHB prescaler
	return ( MCAL_RCC_GetSYSFreq() >> AHB_PrescTable[ ((RCC->CFGR >>4) & 0xF) ] );


}
uint32_t MCAL_RCC_GetPCLK1Freq(void)
{
	//Bits 10:8 PPRE1: APB high-speed prescaler (APB1)
	return ( MCAL_RCC_GetHCLFreq() >> APB1_PrescTable[ ((RCC->CFGR>>8) & 0b111) ] );
}
uint32_t MCAL_RCC_GetPCLK2Freq(void)
{
	//Bits 13:11 PPRE2: APB high-speed prescaler (APB2)
	return ( MCAL_RCC_GetHCLFreq() >> APB2_PrescTable[ ((RCC->CFGR>>11) & 0b111) ] );
}
