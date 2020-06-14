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

EmbeverConfig_TypeDef EmbeverStruct;
Heap_TypeDef HeapStruct_SRAM1;

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
	
//	DelayTimer_Init(TIM2_Period);

	SRAM_Initilization();
//	stSramInit(&HeapStruct_SRAM1, STM32F4XX_eSRAM_START, STM32F4XX_eSRAM_SIZE);
	stSramInit(&HeapStruct_SRAM1, STM32F4XX_SRAM1_START, STM32F4XX_SRAM1_SIZE);
	
	Device_SetDefault();
	
	UartRxBufferPointer_Init();
	
	UART_Init(EmbeverStruct.uartdev.BaudRate, EmbeverStruct.uartdev.WordLength,  EmbeverStruct.uartdev.StopBits, EmbeverStruct.uartdev.Parity, EmbeverStruct.uartdev.HardwareFlowControl);

	ETH_BSP_Config();

	LwIP_Init();
	
	taskENTER_CRITICAL();
	
	xReturn = xTaskCreate((TaskFunction_t)SegmentProcess_Task,
						(const char*)"SegmentProcess_Task",
						(uint32_t)SegmentProcess_Task_STACK_SIZE,
						(void*)NULL,
						(UBaseType_t)SegmentProcess_Task_PRIORITY,
						(TaskHandle_t*)&SegmentProcess_Task_Handle);
	if(pdPASS == xReturn){}
	else{printf("SegmentProcess_Task fail!\r\n");}
		
	xReturn = xTaskCreate((TaskFunction_t)UART1_Receive_Task,
						(const char*)"UART1_Receive_Task",
						(uint32_t)UART1_Receive_Task_STACK_SIZE,
						(void*)NULL,
						(UBaseType_t)UART1_Receive_Task_PRIORITY,
						(TaskHandle_t*)&UART1_Receive_Task_Handle);
	if(pdPASS == xReturn){}
	else{printf("UART1_Receive_Task fail!\r\n");}

	if(EmbeverStruct.dhcp_enable == ENABLE)
	{
		xReturn	= xTaskCreate((TaskFunction_t) LwipDHCP_Task,
													(const char *)  "LwipDHCP_Task",		/*lint !e971 Unqualified char types are allowed for strings and single characters only. */
													(configSTACK_DEPTH_TYPE) LwipDHCP_Task_STACK_SIZE,
													(void * ) NULL,
													(UBaseType_t) LwipDHCP_Task_PRIORITY,
													(TaskHandle_t *) &LwipDHCP_Task_Handle);
		if(pdPASS == xReturn){}
		else{printf("LwipDHCP_Task fail!\r\n");}
	}
		
	taskEXIT_CRITICAL();
	vTaskDelete(Main_Task_Handle);
  for( ;; ){}
}
/*End of file*********************************************/
