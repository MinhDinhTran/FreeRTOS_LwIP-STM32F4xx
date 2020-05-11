#ifndef __MAIN_TASK_H
#define __MAIN_TASK_H

/* Includes ------------------------------------------------------------------*/
#include "task.h"
/*Task Stack Size-------------------------------------------------------------*/
#define	Main_TASK_STACK_SIZE												 (configMINIMAL_STACK_SIZE*4)
#define TCPSever_Task_STACK_SIZE										 (configMINIMAL_STACK_SIZE*4)
#define UART1_Receive_Task_STACK_SIZE								 (configMINIMAL_STACK_SIZE*4)

/*Priority from low to high---------------------------------------------------*/
#define  Main_TASK_PRIORITY            		 					 (configMAX_PRIORITIES - 9)/*this define was used in main.c*/
#define  TCPSever_Task_PRIORITY											 (configMAX_PRIORITIES - 5)
#define  netifINTERFACE_TASK_PRIORITY			           (configMAX_PRIORITIES - 4)
#define	 UART1_Receive_Task_PRIORITY				         (configMAX_PRIORITIES - 2)

extern TaskHandle_t Main_Task_Handle;

extern void Main_Task(void);

#endif /* __MAIN_TASK_H */

