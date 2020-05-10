#ifndef __STM32F4XX_ETH_BSP_H
#define __STM32F4XX_ETH_BSP_H

#include "lwip/netif.h"

#define LAN8720_PHY_ADDRESS  	0x00					//3.7.1 PHYAD[0]: PHY Address Configuration
#define LAN8720_RST 		   		PAout(0) 			//LAN8720¸´Î»Òý½Å

/* Ethernet Flags for EthStatus variable */   
#define ETH_INIT_FLAG           0x01 /* Ethernet Init Flag */
#define ETH_LINK_FLAG           0x10 /* Ethernet Link Flag */

extern __IO uint32_t  EthStatus;

extern void ETH_BSP_Config(void);
extern void EthRecvMDA_IRQ(void);
extern void ETH_link_callback(struct netif *netif);
uint8_t Eth_GetLinkStatus(void);
#endif /* __STM32F407_ETH_BSP_H */
