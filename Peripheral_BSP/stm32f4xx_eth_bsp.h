/**
  ******************************************************************************
  * @author  Lanceli
  * @version V1.0.0
  * @date    09-May-2020
  * @brief   Ethernet low level content initialization
	*          the MAC PHY is LAN
  *          Enable GPIO, DMA on RMII interface
  ******************************************************************************
  * @attention
  * This project is for learning only. If it is for commercial use, please contact the author.
  *
	* website: developerlab.cn
	*
  *Copyright (c) 2020 Lanceli All rights reserved.
  ******************************************************************************
  */
#ifndef __STM32F4XX_ETH_BSP_H
#define __STM32F4XX_ETH_BSP_H

#include "stm32f4xx.h"
#include "stm32f4x7_eth.h"


#define LAN8720_PHY_ADDRESS  	0x00					//3.7.1 PHYAD[0]: PHY Address Configuration


/* MAC PHY Reset pin define
 * The reset pin of LAN8720
*/
#if   defined(STM32F405Rxxx)
#define LAN8720_RST 		   		PAout(0)
#elif defined(STM32F407Rxxx)
#define LAN8720_RST 		   		PAout(5)
#endif


/* Ethernet Flags for EthStatus variable */
#define ETH_INIT_FLAG           0x01 /* Ethernet Init Flag */
#define ETH_LINK_FLAG           0x10 /* Ethernet Link Flag */

extern volatile uint32_t EthStatus;

extern void ETH_BSP_Config(void);
extern void EthRecvMDA_IRQ(void);
uint8_t Eth_GetLinkStatus(void);
#endif /* __STM32F407_ETH_BSP_H */
