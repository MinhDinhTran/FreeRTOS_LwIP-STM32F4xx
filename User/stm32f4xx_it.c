/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    04-November-2016
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "main.h"

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
__weak void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
__weak void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
__weak void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/
void ETH_IRQHandler(void)
{
	/* Frame received */
  if ( ETH_GetDMAFlagStatus(ETH_DMA_FLAG_R) == SET) 
  {
		/* Clear the interrupt flags. */
		/* Clear the Eth DMA Rx IT pending bits */
		ETH_DMAClearITPendingBit(ETH_DMA_IT_R|ETH_DMA_IT_NIS);
		EthRecvMDA_IRQ();
	}
}

#ifdef	UART_IT
void USART1_IRQHandler(void)
{
#ifdef	UART_IT_IDLE
	uint8_t clear;
#endif
#ifdef	UART_IT_RXNE
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
//		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		UartRecv_RXNE_IRQ();
	}
#endif
#ifdef	UART_IT_IDLE
	if(USART_GetITStatus(USART1, USART_IT_IDLE) == SET)
	{
		clear = USART1->SR;
    clear = USART1->DR;
		clear = clear;
		USART_ClearITPendingBit(USART1, USART_IT_IDLE);
		UartRecv_IDLE_IRQ();
	}
#endif
#ifdef	UART_IT_TC
	if(USART_GetITStatus(USART1, USART_IT_TC) == SET)
	{
		USART_ClearITPendingBit(USART1, USART_IT_TC);
		UartTransmit_IRQ();
	}
#endif	
}

void DMA2_Stream5_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA2_Stream5, DMA_IT_TCIF5) == SET)
	{
		DMA_ClearITPendingBit(DMA2_Stream5, DMA_IT_TCIF5);
		DmaComplete_IRQ();
	}
}
#endif


/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/