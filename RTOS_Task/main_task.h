#ifndef __MAIN_TASK_H
#define __MAIN_TASK_H

/* Includes ------------------------------------------------------------------*/
#include "task.h"
/*Task Stack Size-------------------------------------------------------------*/
#define	Main_TASK_STACK_SIZE												 (configMINIMAL_STACK_SIZE*2)
#define SegmentProcess_Task_STACK_SIZE							 (configMINIMAL_STACK_SIZE*2)
#define UART1_Receive_Task_STACK_SIZE								 (configMINIMAL_STACK_SIZE*2)
#define SerialToEthernet_Task_STACK_SIZE             (configMINIMAL_STACK_SIZE*2)
#define LwipDHCP_Task_STACK_SIZE										 (configMINIMAL_STACK_SIZE*2)
/*Priority from low to high---------------------------------------------------*/
#define  Main_TASK_PRIORITY            		 					 (configMAX_PRIORITIES - 9)/*this define was used in main.c*/
#define  LwipDHCP_Task_PRIORITY											 (configMAX_PRIORITIES - 7)
#define  SegmentProcess_Task_PRIORITY								 (configMAX_PRIORITIES - 6)
#define  SerialToEthernet_TCPSever_Task_PRIORITY		 (configMAX_PRIORITIES - 5)
#define  SerialToEthernet_TCPClient_Task_PRIORITY		 (configMAX_PRIORITIES - 5)
#define  netifINTERFACE_TASK_PRIORITY			           (configMAX_PRIORITIES - 4)
#define	 UART1_Receive_Task_PRIORITY				         (configMAX_PRIORITIES - 2)
					
extern TaskHandle_t Main_Task_Handle;

extern void Main_Task(void);

#endif /* __MAIN_TASK_H */

