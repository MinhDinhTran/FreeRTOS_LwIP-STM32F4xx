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
  ******************************************************************************
  */
#include "main.h"
#include "lwip/ip_addr.h"
#include "lwip/netif.h"
#include "netif/ethernet.h"

ETH_InitTypeDef ETH_InitStructure;

volatile uint32_t EthStatus = 0;

extern xSemaphoreHandle s_xSemaphore;
extern struct netif xnetif;
/* Private function prototypes -----------------------------------------------*/
static void ETH_GPIO_Config(void);
static void ETH_NVIC_Config(void);
static void ETH_MACDMA_Config(void);

/**
  * @brief  ETH_BSP_Config
  * @param  None
  * @retval None
  */
void ETH_BSP_Config(void)
{	
  /* Configure the GPIO ports for ethernet pins */
  ETH_GPIO_Config();
	
	LAN8720_RST=0;
	Delay(50);
	LAN8720_RST=1;
	
  /* Config NVIC for Ethernet */
  ETH_NVIC_Config();

  /* Configure the Ethernet MAC/DMA */
  ETH_MACDMA_Config();
		
	if(Eth_GetLinkStatus())
	{
#ifdef ETH_DEBUG
		printf("MAC PHY IS LINKED\r\n");
#endif
	}		
	else
	{
#ifdef ETH_DEBUG
		printf("MAC PHY IS UNLINKED\r\n");
#endif
	}
}


/**
  * @brief  Configures the different GPIO ports.
  * @param  None
  * @retval None
  */
void ETH_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  
	SYSCFG_ETH_MediaInterfaceConfig(SYSCFG_ETH_MediaInterface_RMII);

	/* RMII
	  ETH_MDIO -------------------------> PA2
	  ETH_MDC --------------------------> PC1
	  ETH_RMII_REF_CLK------------------> PA1
	  ETH_RMII_CRS_DV ------------------> PA7
	  ETH_RMII_RXD0 --------------------> PC4
	  ETH_RMII_RXD1 --------------------> PC5
	  ETH_RMII_TX_EN -------------------> PB11
	  ETH_RMII_TXD0 --------------------> PB12
	  ETH_RMII_TXD1 --------------------> PB13
	  ETH_RESET-------------------------> PA0*/
					
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_ETH);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource1, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource4, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource5, GPIO_AF_ETH);
                                
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_ETH);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/**
  * @brief  Configures and enable the Ethernet global interrupt.
  * @param  None
  * @retval None
  */
void ETH_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = ETH_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ETH_IRQnPriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Configures the Ethernet Interface
  * @param  None
  * @retval None
  */
void ETH_MACDMA_Config(void)
{	
	ETH_InitTypeDef ETH_InitStructure;
	
	/* Enable ETHERNET clock  */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_ETH_MAC | RCC_AHB1Periph_ETH_MAC_Tx |RCC_AHB1Periph_ETH_MAC_Rx, ENABLE);
  /* Reset ETHERNET on AHB Bus */     
	ETH_DeInit();
	/* Software reset */
	ETH_SoftwareReset();
	/* Wait for software reset */
	while (ETH_GetSoftwareResetStatus() == SET);
	
	/* ETHERNET Configuration --------------------------------------------------*/
	/* Call ETH_StructInit if you don't like to configure all ETH_InitStructure parameter */
	ETH_StructInit(&ETH_InitStructure);
	
  /* Fill ETH_InitStructure parametrs */
  /*------------------------   MAC   -----------------------------------*/
	ETH_InitStructure.ETH_AutoNegotiation = ETH_AutoNegotiation_Enable;
	ETH_InitStructure.ETH_LoopbackMode = ETH_LoopbackMode_Disable;
	ETH_InitStructure.ETH_RetryTransmission = ETH_RetryTransmission_Disable;
	ETH_InitStructure.ETH_AutomaticPadCRCStrip = ETH_AutomaticPadCRCStrip_Disable;
	ETH_InitStructure.ETH_ReceiveAll = ETH_ReceiveAll_Disable;
	ETH_InitStructure.ETH_BroadcastFramesReception = ETH_BroadcastFramesReception_Enable;
	ETH_InitStructure.ETH_PromiscuousMode = ETH_PromiscuousMode_Disable;
	ETH_InitStructure.ETH_MulticastFramesFilter = ETH_MulticastFramesFilter_Perfect;
	ETH_InitStructure.ETH_UnicastFramesFilter = ETH_UnicastFramesFilter_Perfect;
#ifdef CHECKSUM_BY_HARDWARE
	ETH_InitStructure.ETH_ChecksumOffload = ETH_ChecksumOffload_Enable;
#endif

  /*------------------------   DMA   -----------------------------------*/  
  
  /* When we use the Checksum offload feature, we need to enable the Store and Forward mode: 
  the store and forward guarantee that a whole frame is stored in the FIFO, so the MAC can insert/verify the checksum, 
  if the checksum is OK the DMA can handle the frame otherwise the frame is dropped */
	ETH_InitStructure.ETH_DropTCPIPChecksumErrorFrame = ETH_DropTCPIPChecksumErrorFrame_Enable;
	ETH_InitStructure.ETH_ReceiveStoreForward = ETH_ReceiveStoreForward_Enable; 
	ETH_InitStructure.ETH_TransmitStoreForward = ETH_TransmitStoreForward_Enable; 

	ETH_InitStructure.ETH_ForwardErrorFrames = ETH_ForwardErrorFrames_Disable;
	ETH_InitStructure.ETH_ForwardUndersizedGoodFrames = ETH_ForwardUndersizedGoodFrames_Disable;
	ETH_InitStructure.ETH_SecondFrameOperate = ETH_SecondFrameOperate_Enable;
	ETH_InitStructure.ETH_AddressAlignedBeats = ETH_AddressAlignedBeats_Enable;
	ETH_InitStructure.ETH_FixedBurst = ETH_FixedBurst_Enable;
	ETH_InitStructure.ETH_RxDMABurstLength = ETH_RxDMABurstLength_32Beat;
	ETH_InitStructure.ETH_TxDMABurstLength = ETH_TxDMABurstLength_32Beat;
	ETH_InitStructure.ETH_DMAArbitration = ETH_DMAArbitration_RoundRobin_RxTx_2_1;
	
	/* Configure Ethernet */
	EthStatus = ETH_Init(&ETH_InitStructure,	LAN8720_PHY_ADDRESS);
	
	if(EthStatus)
	{
#ifdef ETH_DEBUG
		printf("Ethernet initilaztion success\r\n");
#endif
	}
	else
	{
#ifdef ETH_DEBUG
		printf("Ethernet initilaztion failed\r\n");
#endif
	}
	
	/* Enable the Ethernet Rx Interrupt */
	ETH_DMAITConfig(ETH_DMA_IT_NIS|ETH_DMA_IT_R, ENABLE);
}

/**
  * @brief  ethernet receive irq function
  * @param  None
  * @retval None
  */
void EthRecvMDA_IRQ(void)
{
  portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

	/* Give the semaphore to wakeup LwIP task */
	xSemaphoreGiveFromISR( s_xSemaphore, &xHigherPriorityTaskWoken );
	
  /* Switch tasks if necessary. */
  if( xHigherPriorityTaskWoken != pdFALSE )
  {
    portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
  }
}

/* Get Ethernet link status*/
uint8_t Eth_GetLinkStatus(void)
{
  if(ETH_ReadPHYRegister(LAN8720_PHY_ADDRESS, LAN8720_BSR) & 0x0004)
  {
    EthStatus |= ETH_LINK_FLAG;
		return SET;
  }
	else
	{		
		return RESET;
	}
}

/**
  * @brief  Link callback function, this function is called on change of link status.
  * @param  The network interface
  * @retval None
  */
void ETH_link_callback(struct netif *netif)
{
  __IO uint32_t timeout = 0;
  uint32_t tmpreg,RegValue;
  struct ip4_addr ipaddr;
  struct ip4_addr netmask;
  struct ip4_addr gw;

  if(netif_is_link_up(netif))
  {
    /* Restart the autonegotiation */
    if(ETH_InitStructure.ETH_AutoNegotiation != ETH_AutoNegotiation_Disable)
    {
      /* Reset Timeout counter */
      timeout = 0;

      /* Enable Auto-Negotiation */
      ETH_WritePHYRegister(LAN8720_PHY_ADDRESS, PHY_BCR, PHY_AutoNegotiation);

      /* Wait until the auto-negotiation will be completed */
      do
      {
        timeout++;
      } while (!(ETH_ReadPHYRegister(LAN8720_PHY_ADDRESS, PHY_BSR) & PHY_AutoNego_Complete) && (timeout < (uint32_t)PHY_READ_TO));

      /* Reset Timeout counter */
      timeout = 0;

      /* Read the result of the auto-negotiation */
      RegValue = ETH_ReadPHYRegister(LAN8720_PHY_ADDRESS, LAN8720_PSCSR);
    
      /* Configure the MAC with the Duplex Mode fixed by the auto-negotiation process */
      if((RegValue & 0x0010) != (uint32_t)RESET)
      {
        /* Set Ethernet duplex mode to Full-duplex following the auto-negotiation */
        ETH_InitStructure.ETH_Mode = ETH_Mode_FullDuplex;  
      }
      else
      {
        /* Set Ethernet duplex mode to Half-duplex following the auto-negotiation */
        ETH_InitStructure.ETH_Mode = ETH_Mode_HalfDuplex;
      }
      /* Configure the MAC with the speed fixed by the auto-negotiation process */
      if(RegValue & 0x0008)
      {
        /* Set Ethernet speed to 10M following the auto-negotiation */
        ETH_InitStructure.ETH_Speed = ETH_Speed_100M; 
      }
      else
      {
        /* Set Ethernet speed to 100M following the auto-negotiation */
        ETH_InitStructure.ETH_Speed = ETH_Speed_10M;      
      }

      /*------------------------ ETHERNET MACCR Re-Configuration --------------------*/
      /* Get the ETHERNET MACCR value */  
      tmpreg = ETH->MACCR;

      /* Set the FES bit according to ETH_Speed value */ 
      /* Set the DM bit according to ETH_Mode value */ 
      tmpreg |= (uint32_t)(ETH_InitStructure.ETH_Speed | ETH_InitStructure.ETH_Mode);

      /* Write to ETHERNET MACCR */
      ETH->MACCR = (uint32_t)tmpreg;

      _eth_delay_(ETH_REG_WRITE_DELAY);
      tmpreg = ETH->MACCR;
      ETH->MACCR = tmpreg;
    }

    /* Restart MAC interface */
    ETH_Start();

		IP4_ADDR(&ipaddr,EmbeverStruct.ipdev.localip[0],EmbeverStruct.ipdev.localip[1],EmbeverStruct.ipdev.localip[2],EmbeverStruct.ipdev.localip[3]);
		IP4_ADDR(&netmask,EmbeverStruct.ipdev.netmask[0],EmbeverStruct.ipdev.netmask[1] ,EmbeverStruct.ipdev.netmask[2],EmbeverStruct.ipdev.netmask[3]);
		IP4_ADDR(&gw,EmbeverStruct.ipdev.gateway[0],EmbeverStruct.ipdev.gateway[1],EmbeverStruct.ipdev.gateway[2],EmbeverStruct.ipdev.gateway[3]);
		
    netif_set_addr(&xnetif, &ipaddr , &netmask, &gw);
    
    /* When the netif is fully configured this function must be called.*/
    netif_set_up(&xnetif);
  }
  else
  {
    ETH_Stop();
    /*  When the netif link is down this function must be called.*/
    netif_set_down(&xnetif);
  }
}

/*End of file*********************************************/
