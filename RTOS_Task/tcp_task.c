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

void TCPSever_Task(void *arg)
{
	void *etherBuffer;
	struct netbuf *rawbuf;
	uint16 len;
  struct netconn *conn, *newconn;
  err_t err, accept_err, recv_err;
	static vu8 connect_state = RESET;
	
	etherBuffer = pvPortMalloc(UART_RX_BUFFER_SIZE);
	if(etherBuffer == NULL){}

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
							if(ReadUartRxBufferToEtherBuffer(&RxdBufferStructure, etherBuffer, len))
							{
								netconn_write(newconn, etherBuffer, len, NETCONN_COPY);
								memset(etherBuffer, 0x00, UART_RX_BUFFER_SIZE);
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
/*End of file*********************************************/
