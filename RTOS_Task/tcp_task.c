/**
  ******************************************************************************
  * @author  Lanceli
  * @version V1.0.0
  * @date    09-May-2020
  * @brief   tcp echo task.
  ******************************************************************************
  * @attention
  * This project is for learning only. If it is for commercial use, please contact the author.
	*
	*Copyright (c) 2020 Lanceli All rights reserved.
  ******************************************************************************
  */
#include "main.h"
#include "lwip/api.h"
#include <string.h>

extern UARTBufferTypeDef	RxdBufferStructure;

void SerialToEthernet_TCPSever_Task(void *arg)
{
	void *etherBuffer;
	struct netbuf *rawbuf;
	uint16 len;
  struct netconn *conn, *newconn;
  err_t err, accept_err, recv_err;
	static vu8 connect_state = RESET;
	
//	etherBuffer = pvPortMalloc(UART_RX_BUFFER_SIZE);
  etherBuffer = stSramMalloc(&HeapStruct_SRAM1, UART_RX_BUFFER_SIZE);
	
  LWIP_UNUSED_ARG(arg);
	
  /* Create a new connection identifier. */
  conn = netconn_new(NETCONN_TCP);
	
  if (conn!=NULL)
  {
    /* Bind connection to well known port number 7. */
    err = netconn_bind(conn, NULL, EmbeverStruct.ipdev.localport);
    
    if (err == ERR_OK)
    {
      /* Tell connection to go into listening mode. */
      err = netconn_listen(conn);
			if (err == ERR_OK){}
			
			while(1)
			{
				while(connect_state != SET)
				{
					/* Grab new connection. */
					if(connect_state == RESET)	accept_err = netconn_accept(conn, &newconn);
					if(accept_err == ERR_OK)
					{
						printf("connected\r\n");
						connect_state = SET;
						newconn->recv_timeout = 10;
						break;
					}
					else
					{
						connect_state = RESET;
						netconn_close(newconn);
						netconn_delete(newconn);
					}
				}
				
				if (accept_err == ERR_OK) 
				{
					while(connect_state == SET)
					{
						recv_err = netconn_recv(newconn, &rawbuf);
						switch(recv_err)
						{
							case ERR_OK:
								do 
								{
									netbuf_data(rawbuf, &etherBuffer, &len);
									UartDmaStreamSend(etherBuffer, len);
								}
								while (netbuf_next(rawbuf) >= 0);
								netbuf_delete(rawbuf);
								break;
							case ERR_TIMEOUT:
								break;
							case ERR_CLSD:
								accept_err = ERR_CLSD;
								break;
							case ERR_CONN:
								accept_err = ERR_CONN;
								break;
							default:
								break;
						}
						
						if(accept_err != ERR_OK)	
						{
							netconn_close(newconn);
							netconn_delete(newconn);
							connect_state = RESET;
							break;
						}

						while(RxdBufferStructure.readableLength)
						{
							len = RxdBufferStructure.readableLength;
							if(ReadUartRxBufferToEtherBuffer(&RxdBufferStructure, etherBuffer, len) == SET)
							{
								netconn_write(newconn, etherBuffer, len, NETCONN_COPY);					
							}
							else
							{								
								printf("readbuffer return reset\r\n");
							}
						}
	  				Delay(10);
						if(Eth_GetLinkStatus() != SET)
						{
							connect_state = RESET;
							/* Close connection and discard connection identifier. */
							netconn_close(newconn);
							netconn_delete(newconn);
							break;
						}
					}
			  }
			}
    }
    else
    {
      netconn_delete(newconn);
    }
  }
  else
  {    
  }
}

void SerialToEthernet_TCPClient_Task(void *arg)
{
	void *etherBuffer;
	struct netbuf *rawbuf;
	uint16 len;
  struct netconn *conn;
  err_t connect_err, recv_err;
	static ip_addr_t server_ipaddr;
	static uint16_t server_port;
	static vu8 connect_state = RESET;

//	etherBuffer = pvPortMalloc(UART_RX_BUFFER_SIZE);
  etherBuffer = stSramMalloc(&HeapStruct_SRAM1, UART_RX_BUFFER_SIZE);
	
	LWIP_UNUSED_ARG(arg);
	
	server_port = EmbeverStruct.ipdev.remoteport;
	IP4_ADDR(&server_ipaddr, EmbeverStruct.ipdev.remoteip[0],EmbeverStruct.ipdev.remoteip[1], EmbeverStruct.ipdev.remoteip[2],EmbeverStruct.ipdev.remoteip[3]);
	
	while(1)
	{
		while(connect_state != SET)
		{
			/* Create a new connection identifier. */
      if(connect_state == RESET) conn = netconn_new(NETCONN_TCP);
			connect_err = netconn_connect(conn, &server_ipaddr, server_port);
			if(connect_err == ERR_OK)
			{
				connect_state = SET;
				conn->recv_timeout = 10;
			  printf("connected\r\n");
			  break;
			}
		  else
		  {
				connect_state = RESET;
				netconn_close(conn);
				netconn_delete(conn);
		  }
		}
		
		if(connect_err == ERR_OK)
		{
			while(connect_state == SET)
			{
				recv_err = netconn_recv(conn, &rawbuf);						
				switch(recv_err)
				{
					case ERR_OK:
						do 
						{
							netbuf_data(rawbuf, &etherBuffer, &len);
							UartDmaStreamSend(etherBuffer, len);
						}
						while (netbuf_next(rawbuf) >= 0);
						netbuf_delete(rawbuf);
						break;
					case ERR_TIMEOUT:
						break;
					case ERR_CLSD:						
						connect_err = ERR_CLSD;
						break;
					case ERR_CONN:
						connect_err = ERR_CONN;
						break;
					default:
						break;
				}
			
				if(connect_err != ERR_OK)	
				{
					netconn_close(conn);
					netconn_delete(conn);					
					connect_state = RESET;
					break;
				}
				
				while(RxdBufferStructure.readableLength)
				{
					len = RxdBufferStructure.readableLength;
					if(ReadUartRxBufferToEtherBuffer(&RxdBufferStructure, etherBuffer, len))
					{
						netconn_write(conn, etherBuffer, len, NETCONN_COPY);
					}
				}
				Delay(10);
				if(Eth_GetLinkStatus() != SET)
				{
					connect_state = RESET;
					/* Close connection and discard connection identifier. */
					netconn_close(conn);
					netconn_delete(conn);
					break;
				}
			}
		}
	}	
}
/*End of file*********************************************/
