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
  
#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f4xx.h"


#define SYSCLK_HZ 168

extern void Systick_Init(u8 SYSCLK);
extern void Delay_us(u32 time_us);
extern void Delay_ms(u16 time_ms);

#endif/*__DELAY_H*/

