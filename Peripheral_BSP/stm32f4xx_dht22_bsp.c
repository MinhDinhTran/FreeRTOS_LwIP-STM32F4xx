/**
  * @version V1.0.0
  * @date    09-May-2020
  * @brief   DHT22 drive
  *
  ******************************************************************************
  * @attention
  * This project is for learning only. If it is for commercial use, please contact the author.
	*
	* Copyright (c) 2020 Lanceli All rights reserved.
  ******************************************************************************
  */
 
#include "main.h"


void TIM2_Init_10us(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 10-1;
	TIM_TimeBaseStructure.TIM_Prescaler = 42-1;
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

}

void DHT22_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DHT22, ENABLE);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_DHT22;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIO_DHT22, &GPIO_InitStructure);
	
	GPIO_DHT22_OUT = SET;/*set the pin level high*/
}

u16 DHT22_ReadTemperature(void)
{
	u16 revale;
	
	GPIO_DHT22_OUT = RESET;
	
	GPIO_DHT22_OUT = SET;
	
	return revale;
}

u16 DHT22_ReadHumidity(void)
{
	u16 revale;
	
	return revale;
}

u8 DTH22_ReadOneFrame(u16 temp, u16 humi)
{
	
}

