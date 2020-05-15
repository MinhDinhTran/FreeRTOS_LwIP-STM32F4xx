#include "main.h"


TaskHandle_t SegmentProcess_Task_Handle;
TaskHandle_t SerialToEthernet_TCPClient_Task_Handle;
TaskHandle_t SerialToEthernet_TCPSever_Task_Handle;

/*The module can only operate in one mode£¬
*Cannot be used as a server when it is a TCP client.
*/
void SegmentProcess_Task(void)
{
	BaseType_t xReturn = pdPASS;
	
	switch(EmbeverStruct.work_mode)
	{
		case 00:
			break;
		case TCP_SERVER:
			taskENTER_CRITICAL();	
			xReturn	= xTaskCreate((TaskFunction_t) SerialToEthernet_TCPSever_Task,
														(const char *)  "SerialToEthernet_Task",		/*lint !e971 Unqualified char types are allowed for strings and single characters only. */
														(configSTACK_DEPTH_TYPE) SerialToEthernet_Task_STACK_SIZE,
														(void * ) NULL,
														(UBaseType_t) SerialToEthernet_TCPSever_Task_PRIORITY,
														(TaskHandle_t *) SerialToEthernet_TCPSever_Task_Handle );
			if(pdFAIL == xReturn){}
			else
			{}
			taskEXIT_CRITICAL();
			break;
		case TCP_CLIENT:
			taskENTER_CRITICAL();	
			xReturn	= xTaskCreate((TaskFunction_t) SerialToEthernet_TCPClient_Task,
														(const char *)  "SerialToEthernet_Task",		/*lint !e971 Unqualified char types are allowed for strings and single characters only. */
														(configSTACK_DEPTH_TYPE) SerialToEthernet_Task_STACK_SIZE,
														(void * ) NULL,
														(UBaseType_t) SerialToEthernet_TCPClient_Task_PRIORITY,
														(TaskHandle_t *) SerialToEthernet_TCPClient_Task_Handle );
			if(pdFAIL == xReturn){}
			else
			{}
			taskEXIT_CRITICAL();
			break;
		case 03:
			break;
		default:
			break;
	}
	vTaskDelete(SegmentProcess_Task_Handle);
	while(1)
	{
	}
}
