/**
  ******************************************************************************
  * @author  Lanceli
  * @version V1.0.0
  * @date    09-May-2020
  * @brief   Temperature Process
	*          
  ******************************************************************************
  * @attention
  * This project is for learning only. If it is for commercial use, please contact the author.
	* websit:www.developerlab.cn
	*Copyright (c) 2020 Lanceli All rights reserved.
  ******************************************************************************
  */
	
#include "main.h"
#include <string.h>

TaskHandle_t stTempProcess_Task_Handle;
xSemaphoreHandle Semaphore_temperature = NULL;


static u16 *uhADCxConvertedValue;

static u16 Get_RawADC(void);


/**
  * @brief  MCU Temperature Process
  * @param  None
  * @retval None
  */
void stTempProcess_Task(void)
{
//	u8 i,j;
//	u16 temp;
	
//	uhADCxConvertedValue = stSramMalloc(&HeapStruct_SRAM1, TEMP_LEN);
	
	#ifdef TEMP_DEBUG
	printf("uhADCxConvertedValue complete\r\n");
	#endif
	
	/*Create Semaphore of get temperature*/
//	if (Semaphore_temperature == NULL)
//  {
//		vSemaphoreCreateBinary(Semaphore_temperature);
//		xSemaphoreTake( Semaphore_temperature, 0);
//	}
	
	while(1)
	{
//		if(xSemaphoreTake(Semaphore_temperature, Wait_BlockTime_temperature) == pdTRUE)
//		{
//			for(i=0; i<TEMP_LEN; i++)
//			{
//				uhADCxConvertedValue[i] = Get_RawADC();
//			}
//			/*Bubble sorting*/
//			for(j=0; j<TEMP_LEN; j++)
//			{
//				for(i=0; i<TEMP_LEN; i++)
//				{
//					if(uhADCxConvertedValue[i] > uhADCxConvertedValue[i+1])
//					{
//						temp = uhADCxConvertedValue[i];
//						uhADCxConvertedValue[i] = uhADCxConvertedValue[i+1];
//						uhADCxConvertedValue[i+1] = temp;
//					}
//				}
//			}
//			/*Get the median*/
//			temp = uhADCxConvertedValue[TEMP_LEN/2];
			
			/*Vsense = Value°¡3300/4096(mV)
			**Temp = ((Vsense - V25)/Avg_Slope)+25
			**V25 = 0.76V
			**Avg_Slope = 2.5 mV/°„C
			**Temp = (Vsense®C 760)/2500 + 25;
			*/		
			
			/*∏°µ„‘ÀÀ„ø™∆Ù*/
			
//			#ifdef TEMP_DEBUG
//			printf("[temp]:%d\r\n",temp);
//			#endif
//		
//		}
//		Temperature_Take();
		Delay(77);
	}
}

/**
  * @brief  Get ADC Raw Value
  * @param  None
  * @retval None
  */
static u16 Get_RawADC(void)   
{  
	ADC_SoftwareStartConv(ADC1);
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));

	return ADC_GetConversionValue(ADC1);
}



/**
  * @brief  Get Temperature Value
  * @param  None
  * @retval None
  */
void Temperature_Take(void)
{
	if(xSemaphoreGive(Semaphore_temperature))
	{
		
	}
	else
	{
		
	}
}

