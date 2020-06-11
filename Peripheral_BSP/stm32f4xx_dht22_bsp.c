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

static u8 DHT22_Start(void)
{
	GPIO_DHT22_OUT = RESET;	
	Delay_us(550);
	GPIO_DHT22_OUT = SET;
	
	Delay_us(40);
	
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

