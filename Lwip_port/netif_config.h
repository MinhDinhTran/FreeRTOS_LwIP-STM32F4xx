/**
  ******************************************************************************
  * @author  Lanceli
  * @version V1.0.0
  * @date    09-May-2020
  * @brief   netif configuration
  ******************************************************************************
  * @attention
  * This project is for learning only. If it is for commercial use, please contact the author.
  ******************************************************************************
  */
#ifndef __NETIF_CONFIG_H
#define __NETIF_CONFIG_H

#include "arch/cc.h"

#ifdef __cplusplus
extern "C" {
#endif

#define	MAC_1 0x00
#define	MAC_2 0x08
#define	MAC_3 0xdc
#define	MAC_4 0x25
#define	MAC_5 0x26
#define	MAC_6 0x27

#define LOCALIP_1 192
#define LOCALIP_2 168
#define LOCALIP_3 1
#define LOCALIP_4 150
	
#define REMOTEIP_1	192
#define REMOTEIP_2	168
#define REMOTEIP_3	1
#define REMOTEIP_4	100
	
#define NETMARK_1 255
#define NETMARK_2 255
#define NETMARK_3 255
#define NETMARK_4 0

#define GATEWAY_1 192
#define GATEWAY_2 168
#define GATEWAY_3 1
#define GATEWAY_4 1

#define LOCALPORT 5000
#define REMOTELPORT 5000

typedef enum
{
	AT_MODE = 0,
	DATA_MODE = 1,
}START_MODE;

typedef enum
{
	TCP_SERVER = 1,
	TCP_CLIENT = 2,
}WORK_MODE;

typedef struct  
{
	uint8 mac[6];
	uint8 remoteip[4];
	uint8 localip[4];
	uint8 netmask[4];
	uint8 gateway[4];
	uint16 localport;
	uint16 remoteport;
	
	vuint8 dhcpstatus;			//dhcp state:
}__lwip_dev;

typedef struct  
{
	uint32 BaudRate;
	uint16 StopBits;
	uint16 Parity;
	uint16 HardwareFlowControl;
}__uart_dev;

typedef struct
{
	__lwip_dev ipdev;
	__uart_dev uartdev;
	
	uint8 dhcp_enable;
	uint8 work_mode;
	uint8 start_state;
}EmbeverConfig_TypeDef;

extern EmbeverConfig_TypeDef EmbeverStruct;

extern void LwIP_Init(void);
#ifdef __cplusplus
}
#endif

#endif /* __NETIF_CONFIG_H */
