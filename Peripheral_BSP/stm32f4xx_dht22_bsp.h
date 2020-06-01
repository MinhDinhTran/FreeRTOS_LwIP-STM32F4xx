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
  
#ifndef __STM32F4XX_DHT22_BSP_H
#define __STM32F4XX_DHT22_BSP_H

#ifdef __cplusplus
extern "C" {
#endif

/**set the PC10 as the data BUS**/
#define RCC_AHB1Periph_DHT22  RCC_AHB1Periph_GPIOC	
#define	GPIO_DHT22						GPIOC
#define	GPIO_Pin_DHT22        GPIO_Pin_10

#define GPIO_DHT22_OUT        PCout(10)
#define GPIO_DHT22_IN         PCin(10)
	

	
#ifdef __cplusplus
}
#endif
#endif /* __STM32F4XX_DHT22_BSP_H */
