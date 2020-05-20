#include "main.h"
#include "lwip/dhcp.h"

extern struct netif xnetif; /* network interface structure */ 
struct dhcp xnetif_dhcp;

TaskHandle_t LwipDHCP_Task_Handle;

void LwipDHCP_Task(void)
{
	struct ip4_addr ipaddr;
  struct ip4_addr netmask;
  struct ip4_addr gw;  	
	static __IO uint8_t DHCP_state = DHCP_START;
	
	uint32 IPaddress;
	uint32 SNmark;
	uint32 GWaddress;
	
	dhcp_set_struct(&xnetif, &xnetif_dhcp);
  
	/* IP address should be set to 0 
		 every time we want to assign a new DHCP address*/
	xnetif_dhcp.offered_ip_addr.addr = 0;
	xnetif_dhcp.offered_gw_addr.addr = 0;
	xnetif_dhcp.offered_sn_mask.addr = 0;
	
  while(1)
  {
    switch (DHCP_state)
    {
			case DHCP_START:
			{				
				dhcp_start(&xnetif);
				
				DHCP_state = DHCP_WAIT_ADDRESS;
			}
				break;
      case DHCP_WAIT_ADDRESS:
      {				
				IPaddress = xnetif_dhcp.offered_ip_addr.addr;
				SNmark = xnetif_dhcp.offered_sn_mask.addr;
				GWaddress = xnetif_dhcp.offered_gw_addr.addr;
				
        if (xnetif_dhcp.offered_ip_addr.addr != 0) 
        {
          DHCP_state = DHCP_ADDRESS_ASSIGNED;
          /* Stop DHCP */
          dhcp_stop(&xnetif);				
					
					EmbeverStruct.ipdev.localip[3]=(uint8_t)(IPaddress>>24); 
					EmbeverStruct.ipdev.localip[2]=(uint8_t)(IPaddress>>16);
					EmbeverStruct.ipdev.localip[1]=(uint8_t)(IPaddress>>8);
					EmbeverStruct.ipdev.localip[0]=(uint8_t)(IPaddress);
					
					printf("IP:%d.%d.%d.%d\r\n",EmbeverStruct.ipdev.localip[0],EmbeverStruct.ipdev.localip[1],EmbeverStruct.ipdev.localip[2],EmbeverStruct.ipdev.localip[3]);

					EmbeverStruct.ipdev.netmask[3]=(uint8_t)(SNmark>>24);
					EmbeverStruct.ipdev.netmask[2]=(uint8_t)(SNmark>>16);
					EmbeverStruct.ipdev.netmask[1]=(uint8_t)(SNmark>>8);
					EmbeverStruct.ipdev.netmask[0]=(uint8_t)(SNmark);

					EmbeverStruct.ipdev.gateway[3]=(uint8_t)(GWaddress>>24);
					EmbeverStruct.ipdev.gateway[2]=(uint8_t)(GWaddress>>16);
					EmbeverStruct.ipdev.gateway[1]=(uint8_t)(GWaddress>>8);
					EmbeverStruct.ipdev.gateway[0]=(uint8_t)(GWaddress);

					IP4_ADDR(&ipaddr,EmbeverStruct.ipdev.localip[0],EmbeverStruct.ipdev.localip[1],EmbeverStruct.ipdev.localip[2],EmbeverStruct.ipdev.localip[3]);
					IP4_ADDR(&netmask,EmbeverStruct.ipdev.netmask[0],EmbeverStruct.ipdev.netmask[1] ,EmbeverStruct.ipdev.netmask[2],EmbeverStruct.ipdev.netmask[3]);
					IP4_ADDR(&gw,EmbeverStruct.ipdev.gateway[0],EmbeverStruct.ipdev.gateway[1],EmbeverStruct.ipdev.gateway[2],EmbeverStruct.ipdev.gateway[3]);
																
					netif_set_addr(&xnetif, &ipaddr , &netmask, &gw);
					
					vTaskDelete(LwipDHCP_Task_Handle);
        }
        else
        {
          /* DHCP timeout */
          if (xnetif_dhcp.tries > MAX_DHCP_TRIES)
          {
            DHCP_state = DHCP_TIMEOUT;
						
            /* Stop DHCP */
            dhcp_stop(&xnetif);
												
            /* Static address used */						
						IP4_ADDR(&ipaddr, EmbeverStruct.ipdev.localip[0],EmbeverStruct.ipdev.localip[1],EmbeverStruct.ipdev.localip[2],EmbeverStruct.ipdev.localip[3]);
						IP4_ADDR(&netmask, EmbeverStruct.ipdev.netmask[0],EmbeverStruct.ipdev.netmask[1] ,EmbeverStruct.ipdev.netmask[2],EmbeverStruct.ipdev.netmask[3]);
						IP4_ADDR(&gw, EmbeverStruct.ipdev.gateway[0],EmbeverStruct.ipdev.gateway[1],EmbeverStruct.ipdev.gateway[2],EmbeverStruct.ipdev.gateway[3]);
																	
            netif_set_addr(&xnetif, &ipaddr , &netmask, &gw);
						
						vTaskDelete(LwipDHCP_Task_Handle);
          }
        }
      }
      break;
      default: 
				break;
    }
    /* wait 250 ms */
    vTaskDelay(250);
  }   
}
