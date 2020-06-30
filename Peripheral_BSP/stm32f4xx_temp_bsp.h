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
	
	
#ifndef __STM32F4XX_TEMP_BSP_H
#define __STM32F4XX_TEMP_BSP_H

#include "stm32f4xx.h"


#define  TEMP_LEN          101
#define  ADCx_DR_ADDRESS   0x4001204c


extern void stTemperature_Init(void);

#endif/*__STM32F4XX_TEMP_BSP_H*/
