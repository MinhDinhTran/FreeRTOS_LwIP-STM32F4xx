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
	
#ifndef __TEMPERATURE_TASK_H
#define __TEMPERATURE_TASK_H

#define	Wait_BlockTime_temperature  100


extern TaskHandle_t stTempProcess_Task_Handle;

extern void stTempProcess_Task(void);
extern void Temperature_Take(void);
#endif/*__TEMPERATURE_TASK_H*/
