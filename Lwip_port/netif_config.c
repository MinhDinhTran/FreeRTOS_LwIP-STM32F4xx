/**
  ******************************************************************************
  * @author  Lanceli
  * @version V1.0.0
  * @date    09-May-2020
  * @brief   netif initialization
  ******************************************************************************
  * @attention
  * This project is for learning only. If it is for commercial use, please contact the author.
	*
	*Copyright (c) 2020 Lanceli All rights reserved.
  ******************************************************************************
  */
#include <string.h>
#include "main.h"
#include "lwip/ip_addr.h"
#include "lwip/api.h"
#include "lwip/tcpip.h"
#include "lwip/etharp.h"
#include "netif_config.h"

/* Define those to better describe your network interface. */
#define IFNAME0 's'
#define IFNAME1 't'

#define netifGUARD_BLOCK_TIME					    ( 250 )
#define netifINTERFACE_TASK_STACK_SIZE		( configMINIMAL_STACK_SIZE*4 )
#define emacBLOCK_TIME_WAITING_FOR_INPUT	( ( portTickType ) 5 )

struct netif xnetif; /* network interface structure */ 
static struct netif *s_pxNetIf = NULL;

xSemaphoreHandle s_xSemaphore = NULL;

uint8_t *Ether_Tx_Buff = NULL;
uint8_t *Ether_Rx_Buff = NULL;
/* Global pointer for last received frame infos */
extern ETH_DMA_Rx_Frame_infos *DMA_RX_FRAME_infos;
/* Global pointers to track current transmit and receive descriptors */
extern ETH_DMADESCTypeDef  *DMATxDescToSet;
extern ETH_DMADESCTypeDef  *DMARxDescToGet;
/* Ethernet Rx & Tx DMA Descriptors */

//extern ETH_DMADESCTypeDef  DMARxDscrTab[ETH_RXBUFNB];/* Ethernet Rx MA Descriptor */
//extern ETH_DMADESCTypeDef  DMATxDscrTab[ETH_TXBUFNB];/* Ethernet Tx DMA Descriptor */

ETH_DMADESCTypeDef  *DMARxDscrTab;
ETH_DMADESCTypeDef  *DMATxDscrTab;
static err_t ethernetif_init(struct netif *netif);
static struct pbuf * low_level_input(struct netif *netif);

static void ip_default(EmbeverConfig_TypeDef *p)
{
	p->ipdev.mac[0] = MAC_1;
	p->ipdev.mac[1] = MAC_2;
	p->ipdev.mac[2] = MAC_3;
	p->ipdev.mac[3] = MAC_4;
	p->ipdev.mac[4] = MAC_5;
	p->ipdev.mac[5] = MAC_6;
	
	p->ipdev.remoteip[0]=REMOTEIP_1;
	p->ipdev.remoteip[1]=REMOTEIP_2;
	p->ipdev.remoteip[2]=REMOTEIP_3;
	p->ipdev.remoteip[3]=REMOTEIP_4;
	
	p->ipdev.localip[0]=LOCALIP_1;
	p->ipdev.localip[1]=LOCALIP_2;
	p->ipdev.localip[2]=LOCALIP_3;
	p->ipdev.localip[3]=LOCALIP_4;
	
	p->ipdev.netmask[0]=NETMARK_1;
	p->ipdev.netmask[1]=NETMARK_2;
	p->ipdev.netmask[2]=NETMARK_3;
	p->ipdev.netmask[3]=NETMARK_4;
	
	p->ipdev.gateway[0]=GATEWAY_1;	
	p->ipdev.gateway[1]=GATEWAY_2;
	p->ipdev.gateway[2]=GATEWAY_3;
	p->ipdev.gateway[3]=GATEWAY_4;
	
	p->ipdev.localport = LOCALPORT;
	p->ipdev.remoteport = REMOTELPORT;
	
	p->ipdev.dhcpstatus=0;
}

/**
  * @brief  Initializes the lwIP stack
  * @param  None
  * @retval None
  */
void LwIP_Init(void)
{
	struct netif * renetif;
	struct ip4_addr ipaddr;
  struct ip4_addr netmask;
  struct ip4_addr gw;
		
	Ether_Tx_Buff = stSramMalloc(&HeapStruct_SRAM1, ETH_TXBUFNB*ETH_TX_BUF_SIZE);			
	Ether_Rx_Buff = stSramMalloc(&HeapStruct_SRAM1, ETH_TXBUFNB*ETH_TX_BUF_SIZE);
	
	DMARxDscrTab = stSramMalloc(&HeapStruct_SRAM1, ETH_TXBUFNB*sizeof(ETH_DMADESCTypeDef));
	DMATxDscrTab = stSramMalloc(&HeapStruct_SRAM1, ETH_TXBUFNB*sizeof(ETH_DMADESCTypeDef));
	
	tcpip_init(NULL,NULL);
	
	ip_default(&EmbeverStruct);
	
	IP4_ADDR(&ipaddr,EmbeverStruct.ipdev.localip[0],EmbeverStruct.ipdev.localip[1],EmbeverStruct.ipdev.localip[2],EmbeverStruct.ipdev.localip[3]);
	IP4_ADDR(&netmask,EmbeverStruct.ipdev.netmask[0],EmbeverStruct.ipdev.netmask[1] ,EmbeverStruct.ipdev.netmask[2],EmbeverStruct.ipdev.netmask[3]);
	IP4_ADDR(&gw,EmbeverStruct.ipdev.gateway[0],EmbeverStruct.ipdev.gateway[1],EmbeverStruct.ipdev.gateway[2],EmbeverStruct.ipdev.gateway[3]);
	
	renetif = netif_add(&xnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &tcpip_input);
	
	if(renetif != NULL)
	{}
	else
	{}
	/*  Registers the default network interface.*/
  netif_set_default(&xnetif);
	
  if (EthStatus == (ETH_INIT_FLAG | ETH_LINK_FLAG))
  {
    /* Set Ethernet link flag */
    xnetif.flags |= NETIF_FLAG_LINK_UP;

    /* When the netif is fully configured this function must be called.*/
    netif_set_up(&xnetif);
  }
  else
  {
    /*  When the netif link is down this function must be called.*/
    netif_set_down(&xnetif);
#ifdef USE_DHCP
    DHCP_state = DHCP_LINK_DOWN;
#endif /* USE_DHCP */
  }
	/* Set the link callback function, this function is called on change of link status*/
  netif_set_link_callback(&xnetif, ETH_link_callback);
}

/**
* This function is the ethernetif_input task, it is processed when a packet 
* is ready to be read from the interface. It uses the function low_level_input() 
* that should handle the actual reception of bytes from the network
* interface. Then the type of the received packet is determined and
* the appropriate input function is called.
*
* @param netif the lwip network interface structure for this ethernetif
*/
void ethernetif_input( void * pvParameters )
{
  struct pbuf *p;
  
  for( ;; )
  {
    if (xSemaphoreTake( s_xSemaphore, emacBLOCK_TIME_WAITING_FOR_INPUT)==pdTRUE)
    {
TRY_GET_NEXT_FRAME:
      p = low_level_input( s_pxNetIf );
      if(p != NULL)
      {
        if (ERR_OK != s_pxNetIf->input( p, s_pxNetIf))
        {
          pbuf_free(p);
        }
        else
        {
          goto TRY_GET_NEXT_FRAME;
        }
      }
    }
		Delay(10);
  }
}

/**
* In this function, the hardware should be initialized.
* Called from ethernetif_init().
*
* @param netif the already initialized lwip network interface structure
*        for this ethernetif
*/
static void low_level_init(struct netif *netif)
{
  uint32_t i;
	BaseType_t xReturn = pdPASS;

  /* set netif MAC hardware address length */
  netif->hwaddr_len = ETHARP_HWADDR_LEN;

  /* set netif MAC hardware address */
  netif->hwaddr[0] =  EmbeverStruct.ipdev.mac[0];
  netif->hwaddr[1] =  EmbeverStruct.ipdev.mac[1];
  netif->hwaddr[2] =  EmbeverStruct.ipdev.mac[2];
  netif->hwaddr[3] =  EmbeverStruct.ipdev.mac[3];
  netif->hwaddr[4] =  EmbeverStruct.ipdev.mac[4];
  netif->hwaddr[5] =  EmbeverStruct.ipdev.mac[5];

  /* set netif maximum transfer unit */
  netif->mtu = 1500;

  /* Accept broadcast address and ARP traffic */
  netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;

  s_pxNetIf =netif;

  /* create binary semaphore used for informing ethernetif of frame reception */
  if (s_xSemaphore == NULL)
  {
    vSemaphoreCreateBinary(s_xSemaphore);
    xSemaphoreTake( s_xSemaphore, 0);
  }

  /* initialize MAC address in ethernet MAC */ 
  ETH_MACAddressConfig(ETH_MAC_Address0, netif->hwaddr);

  /* Initialize Tx Descriptors list: Chain Mode */
  ETH_DMATxDescChainInit(DMATxDscrTab, Ether_Tx_Buff, ETH_TXBUFNB);
  /* Initialize Rx Descriptors list: Chain Mode  */
  ETH_DMARxDescChainInit(DMARxDscrTab, Ether_Rx_Buff, ETH_RXBUFNB);

  /* Enable Ethernet Rx interrrupt */
  {
    for(i=0; i<ETH_RXBUFNB; i++)
    {
      ETH_DMARxDescReceiveITConfig(&DMARxDscrTab[i], ENABLE);
    }
  }

#ifdef CHECKSUM_BY_HARDWARE
  /* Enable the checksum insertion for the Tx frames */
  {
    for(i=0; i<ETH_TXBUFNB; i++)
    {
      ETH_DMATxDescChecksumInsertionConfig(&DMATxDscrTab[i], ETH_DMATxDesc_ChecksumTCPUDPICMPFull);
    }
  }
#endif
	/****************************Create input function task********************************/
	xReturn = xTaskCreate((TaskFunction_t) ethernetif_input,
												(const char *) "Eth_if",
												(configSTACK_DEPTH_TYPE) netifINTERFACE_TASK_STACK_SIZE,
												(void *) NULL,
												(UBaseType_t) netifINTERFACE_TASK_PRIORITY,
												(TaskHandle_t *) NULL );
	if(pdFAIL == xReturn)
	{}
	else
	{}
  /* Enable MAC and DMA transmission and reception */
	ETH_Start();
}

/**
* This function should do the actual transmission of the packet. The packet is
* contained in the pbuf that is passed to the function. This pbuf
* might be chained.
*
* @param netif the lwip network interface structure for this ethernetif
* @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
* @return ERR_OK if the packet could be sent
*         an err_t value if the packet couldn't be sent
*
* @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
*       strange results. You might consider waiting for space in the DMA queue
*       to become availale since the stack doesn't retry to send a packet
*       dropped because of memory failure (except for the TCP timers).
*/

static err_t low_level_output(struct netif *netif, struct pbuf *p)
{
  static xSemaphoreHandle xTxSemaphore = NULL;
  struct pbuf *q;
  u8 *buffer ;
  __IO ETH_DMADESCTypeDef *DmaTxDesc;
  uint16_t framelength = 0;
  uint32_t bufferoffset = 0;
  uint32_t byteslefttocopy = 0;
  uint32_t payloadoffset = 0;

  if (xTxSemaphore == NULL)
  {
    vSemaphoreCreateBinary (xTxSemaphore);
  }

  if (xSemaphoreTake(xTxSemaphore, netifGUARD_BLOCK_TIME))
  {
    DmaTxDesc = DMATxDescToSet;
    buffer = (u8 *)(DmaTxDesc->Buffer1Addr);	//get the Tx_buffer address
    bufferoffset = 0;

    for(q = p; q != NULL; q = q->next) 
    {
      if((DmaTxDesc->Status & ETH_DMATxDesc_OWN) != (u32)RESET)
      {
        goto error;
      }

      /* Get bytes in current lwIP buffer  */
      byteslefttocopy = q->len;
      payloadoffset = 0;

      /* Check if the length of data to copy is bigger than Tx buffer size*/
      while( (byteslefttocopy + bufferoffset) > ETH_TX_BUF_SIZE )
      {
        /* Copy data to Tx buffer*/
        memcpy( (u8_t*)((u8_t*)buffer + bufferoffset), (u8_t*)((u8_t*)q->payload + payloadoffset), (ETH_TX_BUF_SIZE - bufferoffset) );

        /* Point to next descriptor */
        DmaTxDesc = (ETH_DMADESCTypeDef *)(DmaTxDesc->Buffer2NextDescAddr);

        /* Check if the buffer is available */
        if((DmaTxDesc->Status & ETH_DMATxDesc_OWN) != (u32)RESET)
        {
          goto error;
        }

        buffer = (u8 *)(DmaTxDesc->Buffer1Addr);//get the Tx_buff address

        byteslefttocopy = byteslefttocopy - (ETH_TX_BUF_SIZE - bufferoffset);
        payloadoffset = payloadoffset + (ETH_TX_BUF_SIZE - bufferoffset);
        framelength = framelength + (ETH_TX_BUF_SIZE - bufferoffset);
        bufferoffset = 0;
      }

      /* Copy the remaining bytes */
      memcpy( (u8_t*)((u8_t*)buffer + bufferoffset), (u8_t*)((u8_t*)q->payload + payloadoffset), byteslefttocopy );
      bufferoffset = bufferoffset + byteslefttocopy;
      framelength = framelength + byteslefttocopy;
    }

    /* Prepare transmit descriptors to give to DMA*/
    ETH_Prepare_Transmit_Descriptors(framelength);

    /* Give semaphore and exit */
  error:

    xSemaphoreGive(xTxSemaphore);
  }
  return ERR_OK;
}

/**
* Should allocate a pbuf and transfer the bytes of the incoming
* packet from the interface into the pbuf.
*
* @param netif the lwip network interface structure for this ethernetif
* @return a pbuf filled with the received packet (including MAC header)
*         NULL on memory error
*/
static struct pbuf * low_level_input(struct netif *netif)
{
  struct pbuf *p= NULL, *q;
  u32_t len;
  FrameTypeDef frame;
  u8 *buffer;
  __IO ETH_DMADESCTypeDef *DMARxDesc;
  uint32_t bufferoffset = 0;
  uint32_t payloadoffset = 0;
  uint32_t byteslefttocopy = 0;
  uint32_t i=0;  

  /* get received frame */
  frame = ETH_Get_Received_Frame_interrupt();

  /* Obtain the size of the packet and put it into the "len" variable. */
  len = frame.length;
  buffer = (u8 *)frame.buffer;
  
  if (len > 0)
  {
    /* We allocate a pbuf chain of pbufs from the Lwip buffer pool */
    p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);
  }
  
  if (p != NULL)
  {
    DMARxDesc = frame.descriptor;
    bufferoffset = 0;
    for(q = p; q != NULL; q = q->next)
    {
      byteslefttocopy = q->len;
      payloadoffset = 0;

      /* Check if the length of bytes to copy in current pbuf is bigger than Rx buffer size*/
      while( (byteslefttocopy + bufferoffset) > ETH_RX_BUF_SIZE )
      {
        /* Copy data to pbuf*/
        memcpy( (u8_t*)((u8_t*)q->payload + payloadoffset), (u8_t*)((u8_t*)buffer + bufferoffset), (ETH_RX_BUF_SIZE - bufferoffset));

        /* Point to next descriptor */
        DMARxDesc = (ETH_DMADESCTypeDef *)(DMARxDesc->Buffer2NextDescAddr);
        buffer = (unsigned char *)(DMARxDesc->Buffer1Addr);

        byteslefttocopy = byteslefttocopy - (ETH_RX_BUF_SIZE - bufferoffset);
        payloadoffset = payloadoffset + (ETH_RX_BUF_SIZE - bufferoffset);
        bufferoffset = 0;
      }

      /* Copy remaining data in pbuf */
      memcpy( (u8_t*)((u8_t*)q->payload + payloadoffset), (u8_t*)((u8_t*)buffer + bufferoffset), byteslefttocopy);
      bufferoffset = bufferoffset + byteslefttocopy;
    }
  }
  
  /* Release descriptors to DMA */
  DMARxDesc =frame.descriptor;

  /* Set Own bit in Rx descriptors: gives the buffers back to DMA */
  for (i=0; i<DMA_RX_FRAME_infos->Seg_Count; i++)
  {
    DMARxDesc->Status = ETH_DMARxDesc_OWN;
    DMARxDesc = (ETH_DMADESCTypeDef *)(DMARxDesc->Buffer2NextDescAddr);
  }
  
  /* Clear Segment_Count */
  DMA_RX_FRAME_infos->Seg_Count =0;
  
  /* When Rx Buffer unavailable flag is set: clear it and resume reception */
  if ((ETH->DMASR & ETH_DMASR_RBUS) != (u32)RESET)  
  {
    /* Clear RBUS ETHERNET DMA flag */
    ETH->DMASR = ETH_DMASR_RBUS;
    /* Resume DMA reception */
    ETH->DMARPDR = 0;
  }
  return p;
}

/**
* Should be called at the beginning of the program to set up the
* network interface. It calls the function low_level_init() to do the
* actual setup of the hardware.
*
* This function should be passed as a parameter to netif_add().
*
* @param netif the lwip network interface structure for this ethernetif
* @return ERR_OK if the loopif is initialized
*         ERR_MEM if private data couldn't be allocated
*         any other err_t on error
*/
static err_t ethernetif_init(struct netif *netif)
{
  LWIP_ASSERT("netif != NULL", (netif != NULL));

#if LWIP_NETIF_HOSTNAME
  /* Initialize interface hostname */
  netif->hostname = "lwip";
#endif /* LWIP_NETIF_HOSTNAME */

  netif->name[0] = IFNAME0;
  netif->name[1] = IFNAME1;
	
  netif->output = etharp_output;
  netif->linkoutput = low_level_output;

  /* initialize the hardware */
  low_level_init(netif);

//  etharp_init();
//  sys_timeout(ARP_TMR_INTERVAL, arp_timer, NULL);

  return ERR_OK;
}

/*End of file*********************************************/
