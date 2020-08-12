/**
  ******************************************************************************
  * @author  Lanceli
  * @version V1.0.1
  * @date    09-May-2020
  * @brief   netif initialization
  ******************************************************************************
  * @attention
  * This project is for learning only. If it is for commercial use, please contact the author.
	*
	* website: www.developerlab.cn
	*
	*Copyright (c) 2020 Lanceli All rights reserved.
  ******************************************************************************
  */
	
#ifndef __NETIF_CONFIG_H
#define __NETIF_CONFIG_H

/* Define those to better describe your network interface. */
#define IFNAME0 's'
#define IFNAME1 't'

#define netifGUARD_BLOCK_TIME					    ( 5 )
#define emacBLOCK_TIME_WAITING_FOR_INPUT	( ( portTickType ) 0 )

#define	MAC_1       0x00
#define	MAC_2       0x08
#define	MAC_3       0xdc
#define	MAC_4       0x25
#define	MAC_5       0x26
#define	MAC_6       0x27

#define LOCALIP_1   192
#define LOCALIP_2   168
#define LOCALIP_3   1
#define LOCALIP_4   150
	
#define REMOTEIP_1	192
#define REMOTEIP_2	168
#define REMOTEIP_3	1
#define REMOTEIP_4	100
	
#define NETMARK_1   255
#define NETMARK_2   255
#define NETMARK_3   255
#define NETMARK_4   0

#define GATEWAY_1   192
#define GATEWAY_2   168
#define GATEWAY_3   1
#define GATEWAY_4   1

#define LOCALPORT   5000
#define REMOTELPORT 5000



extern xSemaphoreHandle s_xSemaphore;
#endif /* __NETIF_CONFIG_H */
