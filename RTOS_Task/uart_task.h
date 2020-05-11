#ifndef __UART_TASK_H
#define __UART_TASK_H

#include "arch/cc.h"
#include "task.h"

#define UART_BAUDRATE 115200
#define UART_STOPBITS USART_StopBits_1
#define UART_PARITY USART_Parity_No
#define UART_FLOWCONTROL USART_HardwareFlowControl_None

typedef struct
{
	uint8 *rP;
	uint8 *wP;
	uint16	  FLen;
	uint16		ULen;
}RxBufferTypeDef;
	
typedef struct
{
	uint8 *readP;
	uint8 *wirteP;
	
	uint8 readwriteLock;
	
	uint16		WirteResidualLength;
	uint16		ReadResidualLength;
	uint16		wirteableLength;
	uint16		readableLength;
	uint16		FreeLength;
	uint16		UsedLength;
	
	RxBufferTypeDef	RxBuffer0Structure;
	RxBufferTypeDef	RxBuffer1Structure;
	
	uint16 		uartRecv_Counter;
	uint16		dmaCompleteCounter;
	uint8			dmaReversalValue;
	
	uint32    send_counter;
	uint32    receive_counter;
}UARTBufferTypeDef;


extern TaskHandle_t UART1_Receive_Task_Handle;

extern void UartParam_Config(void);
extern void UART1_Receive_Task(void);
extern void UartRxBufferPointer_Init(void);
extern uint8 WirteToUartRxBufferFromRxBuffer0(UARTBufferTypeDef *p, uint16 length);
extern uint8 WirteToUartRxBufferFromRxBuffer1(UARTBufferTypeDef *p, uint16 length);
extern void ClearRxBuffer0WirtePointer(UARTBufferTypeDef *p, uint16 dmaITCounter);
extern void ClearRxBuffer1WirtePointer(UARTBufferTypeDef *p, uint16 dmaITCounter);
extern uint8 ReadUartRxBufferToEtherBuffer(UARTBufferTypeDef *p, uint8 *EtherBuffer, uint16 length);
extern void UartDmaStreamSend(uint8 *buffer, uint16 length);

#endif /* __UART_TASK_H*/
