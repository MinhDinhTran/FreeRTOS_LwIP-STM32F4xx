/**
  ******************************************************************************
  * @author  Lanceli
  * @version V1.0.0
  * @date    09-May-2020
  * @brief   Task entrance.
	*          Create a major task.
  ******************************************************************************
  * @attention
  * This project is for learning only. If it is for commercial use, please contact the author.
	*
	*Copyright (c) 2020 Lanceli All rights reserved.
  ******************************************************************************
  */
#include "main.h"

TaskHandle_t Main_Task_Handle = NULL;/*Main Task Handle*/
TaskHandle_t TCPSever_Task_Handle = NULL;

static void Device_SetDefault(void)
{
	UartParam_Config();
	
	EmbeverStruct.dhcp_enable = ENABLE;
	EmbeverStruct.start_state = DATA_MODE;
	EmbeverStruct.work_mode = TCP_SERVER;
}

void Main_Task(void)
{
	BaseType_t xReturn = pdPASS;

	Device_SetDefault();
	
	UartRxBufferPointer_Init();
	
	UART_Init(EmbeverStruct.uartdev.BaudRate, EmbeverStruct.uartdev.StopBits, EmbeverStruct.uartdev.Parity, EmbeverStruct.uartdev.HardwareFlowControl);

	ETH_BSP_Config();		

	LwIP_Init();
	
	taskENTER_CRITICAL();
	
	xReturn = xTaskCreate((TaskFunction_t)TCPSever_Task,
						(const char*)"TCPSever_Task",
						(uint32_t)TCPSever_Task_STACK_SIZE,
						(void*)NULL,
						(UBaseType_t)TCPSever_Task_PRIORITY,
						(TaskHandle_t*)&TCPSever_Task_Handle);
	if(pdPASS == xReturn){}
	else{}
		
	xReturn = xTaskCreate((TaskFunction_t)UART1_Receive_Task,
						(const char*)"UART1_Receive_Task",
						(uint32_t)UART1_Receive_Task_STACK_SIZE,
						(void*)NULL,
						(UBaseType_t)UART1_Receive_Task_PRIORITY,
						(TaskHandle_t*)&UART1_Receive_Task_Handle);
	if(pdPASS == xReturn){}
	else{}
		
	taskEXIT_CRITICAL();
	vTaskDelete(Main_Task_Handle);
  for( ;; ){}
}
/*End of file*********************************************/
