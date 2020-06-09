/**
  ******************************************************************************
  * @author  Lanceli
  * @version V1.0.0
  * @date    09-May-2020
  * @brief   Ethernet low level content initialization
    *          Enable GPIO, DMA on RMII interface
  ******************************************************************************
  * @attention
  * This project is for learning only. If it is for commercial use, please contact the author.
    *
    *Copyright (c) 2020 Lanceli All rights reserved.
  ******************************************************************************
  */
#ifndef __STM32F4XX_UART_BSP_H
#define __STM32F4XX_UART_BSP_H

#include <stdio.h>
#include "arch/cc.h"

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

  /*USART buffer size*/
#define UART_RX_BUFFER_SIZE	1024
#define UART_TX_BUFFER_SIZE	1024

#define UART_ETHER_BUFFER_SIZE	1024*4

/*USART interrupt enable*/
#define UART_IT
#define	UART_IT_IDLE
#define UART_IT_RXNE
#define	UART_DMA_IT
#define	UART_DMA_TX
//#define UART_IT_TC
//#define UART_OverSampling

void USART_Config(uint32 boundrate, uint16 StopBits, uint16 Parity, uint16 HardwareFlowControl);
extern void UART_Init(uint32 boundrate, uint16 StopBits, uint16 Parity, uint16 HardwareFlowControl);
extern void UART_Init_115200(void);

void UartRecv_IDLE_IRQ(void);
void UartRecv_RXNE_IRQ(void);
void UartTransmit_IRQ(void);
void DmaComplete_IRQ(void);
#endif/*__STM32F4XX_UART_BSP_H*/
