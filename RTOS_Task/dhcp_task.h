#ifndef __DHCP_TASK_H
#define __DHCP_TASK_H

#ifdef __cplusplus
 extern "C" {
#endif
   
/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
#define DHCP_START                 1
#define DHCP_WAIT_ADDRESS          2
#define DHCP_ADDRESS_ASSIGNED      3
#define DHCP_TIMEOUT               4
#define DHCP_LINK_DOWN             5
	 
#define MAX_DHCP_TRIES 4
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
extern TaskHandle_t LwipDHCP_Task_Handle;
/* Exported functions ------------------------------------------------------- */
extern void LwipDHCP_Task(void);
	 
#ifdef __cplusplus
}
#endif
#endif /* __DHCP_TASK_H */
