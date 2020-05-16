#ifndef __TCP_TASK_H
#define __TCP_TASK_H

#ifdef __cplusplus
extern "C" {
#endif
	
#define Eth_ReceiveBufferLength	256
	
extern void SerialToEthernet_TCPSever_Task(void *arg);
extern void SerialToEthernet_TCPClient_Task(void *arg);

#ifdef __cplusplus
}
#endif
#endif /* __TCP_TASK_H */

