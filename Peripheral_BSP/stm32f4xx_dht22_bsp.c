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

static u32 Timer_10us = 0;

void TIM2_Init_us(u16 period)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = period-1;
	TIM_TimeBaseStructure.TIM_Prescaler = 42-1;
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = TIM2_IRQnPriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM2,ENABLE);
}

void TIM2_IRQ(void)
{
	if(Timer_10us) Timer_10us--;
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
	
	TIM2_Init_us(timer2_period_10us);
}

static u8 DHT22_Start(void)
{
	GPIO_DHT22_OUT = RESET;
	Timer_10us = 55;
	while(Timer_10us);
	GPIO_DHT22_OUT = SET;
	Timer_10us = 4;
	while(Timer_10us);
	
	Timer_10us = 10;
	while((!GPIO_DHT22_IN) && Timer_10us);	
	if(!Timer_10us) return RESET;
	
	Timer_10us = 10;
	while(GPIO_DHT22_IN && Timer_10us);	
	if(!Timer_10us) return RESET;
	
	return SET;
}

static u8 DTH22_GetOnebit(void)
{
	u8 revale;
	
	Timer_10us = 10;
	while(!GPIO_DHT22_IN && Timer_10us);
	if(!Timer_10us) return RESET;
	
	Timer_10us = 4;
	while(GPIO_DHT22_IN && Timer_10us);
	if(Timer_10us) revale = 0;
	else revale = 1;
	
	return revale;
}

static u8 DHT22_GetOneByte(void)
{
	u8 data = 0;
	u8 i;
	
	for(i=0; i<8; i++)
	{
		data <<= 1;
		data |= DTH22_GetOnebit();
	}
	return data;
}


u8 DHT22_GetOneFrame(u8 *result)
{
	u8 revale;
	u8 i;
	u8 data[5];
	
	if(DHT22_Start())
	{
		
	}
	else
	{
		return 0xff;
	}
	
	for(i=0; i<5; i++)
	{
		data[i] = DHT22_GetOneByte();
	}
	
	revale = data[0] + data[1] + data[2] + data[3];
	
	if(revale == data[4])
	{
		for(i=0; i<4; i++)
		{
			*result = data[i];
			result++;
		}
		revale = SET;
	}
	else
	{
		revale = RESET;
	}
	
	return revale;
}

