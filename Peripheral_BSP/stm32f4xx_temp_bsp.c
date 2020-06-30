/**
  ******************************************************************************
  * @author  Lanceli
  * @version V1.0.0
  * @date    09-May-2020
  * @brief   Temperature initialization
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

/**
  * @brief  ADC1_Config
  * @param  None
  * @retval None
  */
static void ADC1_Config(void)
{
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	ADC_TempSensorVrefintCmd(ENABLE);
	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
  ADC_CommonInit(&ADC_CommonInitStructure);
	
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADC1, &ADC_InitStructure);	
}


/**
  * @brief  stTemperature_Init
  * @param  None
  * @retval None
  */
void stTemperature_Init(void)
{	
	ADC1_Config();
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_480Cycles );
	
	ADC_Cmd(ADC1, ENABLE);
}


