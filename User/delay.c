/**
  ******************************************************************************
  * @author  
  * @version V1.0.0
  * @date    09-May-2020
  * @brief   Delay Timer
  *          
  ******************************************************************************
  * @attention
  * This project is for learning only. If it is for commercial use, please contact the author.
	*
	*
  ******************************************************************************
  */
  
#include "main.h"
	
static u8  fac_us = 0;
static u16 fac_ms = 0;

/**
  * @brief  system tick clock initilization
  * @param  None
  * @retval None
  */
void Systick_Init(u8 SYSCLK)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	fac_us = SYSCLK / 8;
	fac_ms = (u16)fac_us * 1000;
}

/**
  * @brief  Microsecond delay function
  * @param  None
  * @retval None
  */
void Delay_us(u32 time_us)
{
	u32 temp;	    	 
	SysTick->LOAD=time_us*fac_us;	 
	SysTick->VAL=0x00;
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;
	SysTick->VAL =0X00;
}

static void delay_xms(u16 nms)
{
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;
	SysTick->VAL =0x00;
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk; 
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;
	SysTick->VAL =0X00;	  	    
}

/**
  * @brief  millisecond delay function
  * @param  None
  * @retval None
  */
void Delay_ms(u16 time_ms)
{	 	 
	u8 repeat=time_ms/540;	
	u16 remain=time_ms%540;
	while(repeat)
	{
		delay_xms(540);
		repeat--;
	}
	if(remain) delay_xms(remain);
}
