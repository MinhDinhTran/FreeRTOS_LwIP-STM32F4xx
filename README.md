这是一个基于STM32F4xx系列芯片的代码框架，在FreeRTOS上搭载LwIP，目的是为了以太网相关开发提供方便的平台。

This is a code framework based on stm32f4xx series chips, which is equipped with LwIP on FreeRTOS. The purpose is to provide a convenient platform for Ethernet related development.

## 硬件部分|Hardware

代码基于两个硬件平台编写：STM32F405RGT6与STM32F407ZGT6。两个EVB的MCU架构都为Cortex-M4，硬件的主要特点如下：

1. MCU晶振频率为168MHz，具比有Cortex-M3处理器更快的运算速率。
2. MCU具有CMM（内核耦合寄存器），将系统内核放置到CMM中，使程序栈调用速率大大提升。
3. MCU具有FPU模块，使得浮点运算速率大大提升，可以优化算法计算速度。

PCB外形如下图所示：

**STM32F405RG-EVB**

![STM32F405](https://github.com/laneston/Pictures/blob/master/PCB-STM32F4xx/STM32F405EVB.jpg)

**STM32F407ZG-EVB**

![STM32F407-A](https://github.com/laneston/Pictures/blob/master/PCB-STM32F4xx/STM32F407EVB.jpg)

![STM32F407-B](https://github.com/laneston/Pictures/blob/master/PCB-STM32F4xx/STM32F407EVB-B.jpg)

代码同时兼容两个EVB，但有稍许的差别：在不同的EVB上，MCU连接MAC PHY芯片LAN8720的RESET引脚，分别是不同的I/O口。使用的时候需要更改宏定义。

Code can run on both EVBs, but make some changes, because the pins have some differents. Fortunately, the only pin need to change in the code that is RESRT pin on MAC PHY of LAN8720, the others are same definition. It mind can be match same base bord.You need to change the macro definition when you use it.

**更改方式如下：**


## 代码框架|Software
<PRE>
STM32F405/407
   |__CMSIS
   |  |__system_stm32f4xx.c [STM32内核配置文件]
   |  |__startup_stm32f40_41xxx.c [STM32F4 启动文件]
   |
   |__STM32F4xx_StdPeriph_Dirver
   |
   |__STM32F4x7_ETH_Driver
   |  |__stm32f4x7_eth.c
   |
   |__FreeRTOS
   |
   |__Lwip_core
   |
   |__Lwip_api
   |
   |__Lwip_netif
   |
   |__Lwip_app
   |
   |__Lwip_port
   |
   |__User
   |  |__main.c
   |  |__stm32f4xx_it.c
   |  |__main.h
   |  |__FreeRTOSConfig.h
   |  |__delay.c
   |
   |__RTOS_Task
   |  |__main_task.c
   |  |__tcp_task.c
   |  |__uaet_task.c
   |  |__segment_task.c
   |  |__dhcp_task.c
   |
   |__Peripheral_BSP
      |__stm32f4xx_eth_bsp.c
      |__stm32f4xx_uart_bsp.c
      |__stm32f4xx_heap_bsp.c
      |__stm32f4xx_fsmc_bsp.c
      |__stm32f4xx_dht22_bsp.c

</PRE>
